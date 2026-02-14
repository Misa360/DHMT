package thaytuan;

import javax.swing.*;
import java.awt.*;
import java.awt.event.MouseAdapter;
import java.awt.event.MouseEvent;

public class BanCoCaro extends JFrame {

    public BanCoCaro() {
        setTitle("Bàn cờ Caro");
        setSize(500, 500);
        setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
        setLocationRelativeTo(null);
        add(new CaroPanel());
    }

    public static void main(String[] args) {
        new BanCoCaro().setVisible(true);
    }
}

class CaroPanel extends JPanel {

    int size = 10;
    int[][] board = new int[size][size];
    boolean isXTurn = true; // lượt X trước

    public CaroPanel() {
        addMouseListener(new MouseAdapter() {
            @Override
            public void mouseClicked(MouseEvent e) {
                int cellSize = getWidth() / size;
                int col = e.getX() / cellSize;
                int row = e.getY() / cellSize;

                if (row < size && col < size && board[row][col] == 0) {
                    board[row][col] = isXTurn ? 1 : 2;
                    isXTurn = !isXTurn;
                    repaint();
                }
            }
        });
    }

    @Override
    protected void paintComponent(Graphics g) {
        super.paintComponent(g);
        int cellSize = getWidth() / size;

        // Vẽ lưới
        g.setColor(Color.BLACK);
        for (int i = 0; i <= size; i++) {
            g.drawLine(i * cellSize, 0, i * cellSize, size * cellSize);
            g.drawLine(0, i * cellSize, size * cellSize, i * cellSize);
        }

        // Vẽ X và O
        for (int r = 0; r < size; r++) {
            for (int c = 0; c < size; c++) {
                if (board[r][c] == 1) {
                    drawX(g, c * cellSize, r * cellSize, cellSize);
                } else if (board[r][c] == 2) {
                    drawO(g, c * cellSize, r * cellSize, cellSize);
                }
            }
        }
    }

    void drawX(Graphics g, int x, int y, int s) {
        g.setColor(Color.RED);
        g.drawLine(x + 10, y + 10, x + s - 10, y + s - 10);
        g.drawLine(x + s - 10, y + 10, x + 10, y + s - 10);
    }

    void drawO(Graphics g, int x, int y, int s) {
        g.setColor(Color.BLUE);
        g.drawOval(x + 10, y + 10, s - 20, s - 20);
    }
}
