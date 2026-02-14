package thaytuan;

import java.awt.*;
import javax.swing.*;

public class VeHinhTron extends JFrame {

    public VeHinhTron() {
        setTitle("Hình tròn ở giữa");
        setSize(400, 400);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);

        add(new VeHinhTronPanel());
    }

    public static void main(String[] args) {
        new VeHinhTron().setVisible(true);
    }
}

class VeHinhTronPanel extends JPanel {

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);

        int duongKinh = 100;

        int width = getWidth();
        int height = getHeight();

        int x = (width - duongKinh) / 2;
        int y = (height - duongKinh) / 2;

        g.setColor(Color.YELLOW);
        g.fillOval(x, y, duongKinh, duongKinh);
    }
}