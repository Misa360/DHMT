using System;
using System.Text;
using System.Security.Cryptography;
using System.IO;
using MySql.Data.MySqlClient;

namespace UrbanSecurityApp
{
    // Lớp hỗ trợ bảo mật: Mã hóa AES và Hash SHA256
    public static class SecurityHelper
    {
        // Chìa khóa AES (Cần giữ bí mật tuyệt đối)
        private static readonly string AesKey = "12345678901234567890123456789012"; // 32 ký tự
        private static readonly string AesIV = "1234567890123456";  // 16 ký tự

        // 1. Mã hóa AES (Cho Số ĐT, Địa chỉ)
        public static string Encrypt(string plainText)
        {
            if (string.IsNullOrEmpty(plainText)) return "";
            using (Aes aes = Aes.Create())
            {
                aes.Key = Encoding.UTF8.GetBytes(AesKey);
                aes.IV = Encoding.UTF8.GetBytes(AesIV);
                var encryptor = aes.CreateEncryptor(aes.Key, aes.IV);
                using (var ms = new MemoryStream())
                {
                    using (var cs = new CryptoStream(ms, encryptor, CryptoStreamMode.Write))
                    {
                        using (var sw = new StreamWriter(cs)) sw.Write(plainText);
                        return Convert.ToBase64String(ms.ToArray());
                    }
                }
            }
        }

        // 2. Hash SHA256 (Cho Mật khẩu)
        public static string HashSHA256(string password, string salt)
        {
            using (SHA256 sha256 = SHA256.Create())
            {
                byte[] bytes = sha256.ComputeHash(Encoding.UTF8.GetBytes(password + salt));
                StringBuilder sb = new StringBuilder();
                foreach (byte b in bytes) sb.Append(b.ToString("x2"));
                return sb.ToString();
            }
        }
    }

    class Program
    {
        static string connString = "Server=localhost;Database=urban_security;Uid=root;Pwd=Buingocanh@234;";

        static void Main(string[] args)
        {
            Console.OutputEncoding = Encoding.UTF8;
            Console.InputEncoding = Encoding.UTF8;

            while (true)
            {
                Console.Clear();
                Console.WriteLine("======= ĐĂNG KÝ HỆ THỐNG AN NINH ĐÔ THỊ (BẢO MẬT CAO) =======");

                // Nhập Account
                Console.Write("Tên đăng nhập: ");
                string user = Console.ReadLine();
                Console.Write("Email: ");
                string email = Console.ReadLine();

                // Kiểm tra Password
                string pass = "";
                while (true)
                {
                    Console.Write("Mật khẩu: ");
                    pass = Console.ReadLine();
                    Console.Write("Xác nhận lại mật khẩu: ");
                    if (pass == Console.ReadLine()) break;
                    Console.WriteLine("❌ Mật khẩu không khớp! Thử lại.");
                }

                // Nhập Profile
                Console.WriteLine("--- Thông tin cá nhân ---");
                Console.Write("Họ và tên: ");
                string name = Console.ReadLine();
                Console.Write("Số điện thoại: ");
                string phone = Console.ReadLine();
                Console.Write("Địa chỉ: ");
                string addr = Console.ReadLine();
                Console.Write("Giới tính (Male/Female/Other): ");
                string gender = Console.ReadLine();

                RegisterUser(user, pass, email, name, phone, addr, gender);

                Console.WriteLine("\nNhấn 'q' để thoát hoặc phím bất kỳ để tiếp tục...");
                if (Console.ReadKey().KeyChar == 'q') break;
            }
        }

        static void RegisterUser(string user, string pass, string email, string name, string phone, string addr, string gender)
        {
            using (MySqlConnection conn = new MySqlConnection(connString))
            {
                try
                {
                    conn.Open();
                    MySqlTransaction trans = conn.BeginTransaction();

                    try
                    {
                        // 1. Xử lý bảo mật
                        string salt = Guid.NewGuid().ToString();
                        string hashedPw = SecurityHelper.HashSHA256(pass, salt);
                        
                        // Mã hóa thông tin nhạy cảm trước khi lưu
                        string encPhone = SecurityHelper.Encrypt(phone);
                        string encAddr = SecurityHelper.Encrypt(addr);

                        // 2. Lưu Accounts
                        string sqlAcc = @"INSERT INTO accounts (username, password_hash, salt, email, role) 
                                          VALUES (@u, @h, @s, @e, 'user'); SELECT LAST_INSERT_ID();";
                        MySqlCommand cmdAcc = new MySqlCommand(sqlAcc, conn, trans);
                        cmdAcc.Parameters.AddWithValue("@u", user);
                        cmdAcc.Parameters.AddWithValue("@h", hashedPw);
                        cmdAcc.Parameters.AddWithValue("@s", salt);
                        cmdAcc.Parameters.AddWithValue("@e", email);
                        int userId = Convert.ToInt32(cmdAcc.ExecuteScalar());

                        // 3. Lưu Profiles
                        string sqlProf = @"INSERT INTO user_profiles (user_id, full_name, phone_number, address, date_of_birth, gender) 
                                           VALUES (@id, @n, @p, @a, @d, @g)";
                        MySqlCommand cmdProf = new MySqlCommand(sqlProf, conn, trans);
                        cmdProf.Parameters.AddWithValue("@id", userId);
                        cmdProf.Parameters.AddWithValue("@n", name);
                        cmdProf.Parameters.AddWithValue("@p", encPhone); // Lưu bản mã hóa
                        cmdProf.Parameters.AddWithValue("@a", encAddr);  // Lưu bản mã hóa
                        cmdProf.Parameters.AddWithValue("@d", DateTime.Now.AddYears(-20));
                        cmdProf.Parameters.AddWithValue("@g", gender);
                        cmdProf.ExecuteNonQuery();

                        trans.Commit();
                        Console.WriteLine("\n✅ Đăng ký thành công! Dữ liệu nhạy cảm đã được mã hóa.");
                    }
                    catch (Exception ex)
                    {
                        trans.Rollback();
                        Console.WriteLine("\n❌ Lỗi xử lý: " + ex.Message);
                    }
                }
                catch (Exception ex)
                {
                    Console.WriteLine("\n❌ Lỗi kết nối MySQL: " + ex.Message);
                }
            }
        }
    }
}