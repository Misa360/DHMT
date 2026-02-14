package thaytuan;

import javax.swing.*;
import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;

public class Bt2 extends JFrame implements ActionListener {
    private JTextField txtA, txtB, txtKetQua;
    private JButton btnCong, btnTru, btnNhan, btnChia, btnExit, btnReset;

    public static void main(String[] args) {
        new Bt2();
    }

    public Bt2() {
        setTitle("Minh hoa cac phep toan");
        setSize(450, 300);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        setLayout(new BorderLayout(10, 10));
        JLabel lblTitle = new JLabel("minh hoa cac phep toan", SwingConstants.CENTER);
        lblTitle.setFont(new Font("Arial", Font.BOLD, 16));
        this.add(lblTitle, BorderLayout.NORTH);

        JPanel pInput = new JPanel(new GridLayout(3, 2, 5, 5));
        pInput.setBorder(BorderFactory.createEmptyBorder(10, 30, 10, 30));

        pInput.add(new JLabel("Nhap a:"));
        txtA = new JTextField();
        pInput.add(txtA);

        pInput.add(new JLabel("Nhap b:"));
        txtB = new JTextField();
        pInput.add(txtB);

        pInput.add(new JLabel("Ket qua:"));
        txtKetQua = new JTextField();
        txtKetQua.setEditable(false);
        pInput.add(txtKetQua);

        this.add(pInput, BorderLayout.CENTER);

        JPanel pButtons = new JPanel(new GridLayout(2, 1));

        JPanel pRow1 = new JPanel();
        btnCong = new JButton("Cong");
        btnTru = new JButton("Tru");
        btnNhan = new JButton("Nhan");
        btnChia = new JButton("Chia");
        pRow1.add(btnCong);
        pRow1.add(btnTru);
        pRow1.add(btnNhan);
        pRow1.add(btnChia);

        JPanel pRow2 = new JPanel();
        btnExit = new JButton("Exit");
        btnReset = new JButton("Reset");
        pRow2.add(btnExit);
        pRow2.add(btnReset);

        pButtons.add(pRow1);
        pButtons.add(pRow2);

        this.add(pButtons, BorderLayout.SOUTH);
        btnCong.addActionListener(this);
        btnTru.addActionListener(this);
        btnNhan.addActionListener(this);
        btnChia.addActionListener(this);
        btnExit.addActionListener(this);
        btnReset.addActionListener(this);

        this.setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        Object source = e.getSource();
        if (source == btnExit) {
            System.exit(0);
        } else if (source == btnReset) {
            txtA.setText("");
            txtB.setText("");
            txtKetQua.setText("");
            txtA.requestFocus();
            return;
        }
        try {
            double a = Double.parseDouble(txtA.getText());
            double b = Double.parseDouble(txtB.getText());
            double kq = 0;

            if (source == btnCong)
                kq = a + b;
            else if (source == btnTru)
                kq = a - b;
            else if (source == btnNhan)
                kq = a * b;
            else if (source == btnChia) {
                if (b == 0) {
                    JOptionPane.showMessageDialog(this, "Không thể chia cho 0!");
                    return;
                }
                kq = a / b;
            }

            txtKetQua.setText(String.valueOf(kq));
        } catch (NumberFormatException ex) {
            JOptionPane.showMessageDialog(this, "Vui lòng nhập số hợp lệ!");
        }
    }
}