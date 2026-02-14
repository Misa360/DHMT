package thaytuan;

import java.awt.*;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.*;

public class Bt3 extends JFrame implements ActionListener {

    private JTextField manhinh;
    private double soThuNhat = 0;
    private String phepToan = "";
    private boolean batDauSoMoi = true;

    public static void main(String[] args) {
        try {
            UIManager.setLookAndFeel(UIManager.getSystemLookAndFeelClassName());
        } catch (Exception e) {
        }
        SwingUtilities.invokeLater(() -> new Bt3());
    }

    public Bt3() {
        setTitle("Calculator");
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setSize(350, 500);
        setLayout(new BorderLayout(5, 5));

        // Màn hình hiển thị
        manhinh = new JTextField("0");
        manhinh.setFont(new Font("Segoe UI", Font.BOLD, 45));
        manhinh.setHorizontalAlignment(JTextField.RIGHT);
        manhinh.setEditable(false);
        manhinh.setBorder(BorderFactory.createEmptyBorder(20, 10, 20, 10));
        manhinh.setBackground(Color.WHITE);
        add(manhinh, BorderLayout.NORTH);

        // Bàn phím
        JPanel banphim = new JPanel(new GridLayout(5, 4, 4, 4));
        banphim.setBackground(new Color(240, 240, 240));
        banphim.setBorder(BorderFactory.createEmptyBorder(5, 5, 5, 5));

        String[] nut = {
                "CE", "C", "\u232B", "÷",
                "7", "8", "9", "x",
                "4", "5", "6", "-",
                "1", "2", "3", "+",
                "+/-", "0", ".", "="
        };

        for (String n : nut) {
            JButton button = new JButton(n);
            button.setFont(new Font("Segoe UI", Font.PLAIN, 18));
            button.setFocusable(false);
            button.setBorderPainted(false);
            button.addActionListener(this);

            // Thiết lập màu sắc theo loại nút
            if ("0123456789.".contains(n)) {
                button.setBackground(Color.WHITE);
            } else if (n.equals("=")) {
                button.setBackground(new Color(0, 120, 215)); // Màu Blue hiện đại
                button.setForeground(Color.WHITE);
            } else {
                button.setBackground(new Color(225, 225, 225));
            }

            banphim.add(button);
        }

        add(banphim, BorderLayout.CENTER);
        setLocationRelativeTo(null);
        setVisible(true);
    }

    @Override
    public void actionPerformed(ActionEvent e) {
        String text = e.getActionCommand();

        if ("0123456789.".contains(text)) {
            if (batDauSoMoi) {
                manhinh.setText(text);
                batDauSoMoi = false;
            } else {
                if (text.equals(".") && manhinh.getText().contains("."))
                    return;
                manhinh.setText(manhinh.getText() + text);
            }
        } else if ("+-x÷".contains(text)) {
            soThuNhat = Double.parseDouble(manhinh.getText());
            phepToan = text;
            batDauSoMoi = true;
        } else if (text.equals("=")) {
            calculate();
        } else if (text.equals("C") || text.equals("CE")) {
            manhinh.setText("0");
            batDauSoMoi = true;
            if (text.equals("C")) {
                soThuNhat = 0;
                phepToan = "";
            }
        } else if (text.equals("\u232B")) { // Nút Backspace
            String s = manhinh.getText();
            if (s.length() > 0) {
                s = s.substring(0, s.length() - 1);
                manhinh.setText(s.isEmpty() ? "0" : s);
            }
        } else if (text.equals("+/-")) {
            double val = Double.parseDouble(manhinh.getText());
            manhinh.setText(String.valueOf(val * -1));
        }
    }

    private void calculate() {
        double soThuHai = Double.parseDouble(manhinh.getText());
        double ketQua = 0;
        switch (phepToan) {
            case "+":
                ketQua = soThuNhat + soThuHai;
                break;
            case "-":
                ketQua = soThuNhat - soThuHai;
                break;
            case "x":
                ketQua = soThuNhat * soThuHai;
                break;
            case "÷":
                if (soThuHai != 0)
                    ketQua = soThuNhat / soThuHai;
                else {
                    JOptionPane.showMessageDialog(this, "Không thể chia cho 0!");
                    return;
                }
                break;
            default:
                return;
        }
        manhinh.setText(String.valueOf(ketQua));
        batDauSoMoi = true;
    }
}