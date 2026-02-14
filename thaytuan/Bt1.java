package thaytuan;

import java.awt.Color;
import java.awt.FlowLayout;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import javax.swing.JButton;
import javax.swing.JFrame;

public class Bt1 extends JFrame {
	public static void main(String[] args) {
		new Bt1();
	}

	public Bt1() {
		this.setTitle("Doi mau nen");
		this.setSize(400, 300);
		this.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);
		this.setLayout(new FlowLayout());

		Bt1 p = this;
		JButton bGreen = new JButton("Green");
		bGreen.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				p.getContentPane().setBackground(Color.GREEN);
			}
		});
		JButton bBlue = new JButton("Blue");
		bBlue.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				p.getContentPane().setBackground(Color.BLUE);
			}
		});
		JButton bRed = new JButton("Red");
		bRed.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				p.getContentPane().setBackground(Color.RED);
			}
		});
		JButton bExit = new JButton("Exit");
		bExit.addActionListener(new ActionListener() {
			@Override
			public void actionPerformed(ActionEvent e) {
				System.exit(0);
			}
		});
		this.add(bGreen);
		this.add(bBlue);
		this.add(bRed);
		this.add(bExit);

		this.setVisible(true);
	}
}