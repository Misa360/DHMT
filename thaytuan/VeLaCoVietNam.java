package thaytuan;

import javax.swing.*;
import java.awt.*;
import java.awt.geom.GeneralPath;

public class VeLaCoVietNam extends JPanel {
    private double rotationAngle = 0;

    public VeLaCoVietNam() {
        Timer timer = new Timer(10, e -> {
            rotationAngle += 1;
            if (rotationAngle >= 360)
                rotationAngle = 0;
            repaint();
        });
        timer.start();
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        Graphics2D g2d = (Graphics2D) g;

        g2d.setRenderingHint(RenderingHints.KEY_ANTIALIASING, RenderingHints.VALUE_ANTIALIAS_ON);

        int width = getWidth();
        int height = getHeight();

        g2d.setColor(Color.RED);
        g2d.fillRect(0, 0, width, height);
        double centerX = width / 2.0;
        double centerY = height / 2.0;
        double radius = height / 4.0;

        drawStar(g2d, centerX, centerY, radius);
    }

    private void drawStar(Graphics2D g2d, double x, double y, double r) {
        double innerRadius = r * Math.sin(Math.toRadians(18)) / Math.sin(Math.toRadians(54));
        GeneralPath star = new GeneralPath();
        for (int i = 0; i < 10; i++) {
            double angle = Math.toRadians(i * 36 - 90 + rotationAngle);
            double currRadius = (i % 2 == 0) ? r : innerRadius;
            double px = x + currRadius * Math.cos(angle);
            double py = y + currRadius * Math.sin(angle);

            if (i == 0)
                star.moveTo(px, py);
            else
                star.lineTo(px, py);
        }
        star.closePath();

        g2d.setColor(Color.YELLOW);
        g2d.fill(star);
    }

    public static void main(String[] args) {
        JFrame frame = new JFrame("Lá cờ Việt Nam Quay");
        frame.add(new VeLaCoVietNam());
        frame.setSize(600, 400);
        frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        frame.setLocationRelativeTo(null);
        frame.setVisible(true);
    }
}