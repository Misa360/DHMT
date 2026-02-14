#include <iostream>
#include <cmath>
using namespace std;
//Solving a cubic equation using the discriminant approach
int main() {
	double a, b, c, d;
cout << "Please enter the values for the a, b, c, d coefficients in order: ";
cin >> a >> b >> c >> d;
double Delta0 = pow(b,2.0) - 3.0*a*c;
double Delta1 = (2.0*pow(b,3.0)- (9.0*a*b*c) + (27.0*pow(a,2.0)*d));
double Disc = (pow(Delta1,2.0) - 4.0*pow(Delta0,3.0))/(27.0*pow(a,2.0));
double C = cbrt(sqrt((pow(Delta1,2.0)- 4.0*pow(Delta1,3.0) + Delta1)/2.0));
double u = (-1.0 + sqrt(-3.0))/2.0;
if(Disc>0){      //Three real solutions
    double x1 = (b+pow(u,1.0)*C+Delta0/(pow(u,1.0)*C))/(3.0*a);
	double x2 = (b+pow(u,2.0)*C+Delta0/(pow(u,2.0)*C))/(3.0*a);
	double x3 = (b+pow(u,3.0)*C+Delta0/(pow(u,3.0)*C))/(3.0*a);
	cout << "D is positive and the roots are: "<< x1 << ", " << x2 << ", " << x3 << endl;	
} else if (Disc==0) {//one or two real solutions
double x1 = (b+pow(u,1.0)*C+Delta0/(pow(u,1.0)*C))/(3.0*a);
double x2 = (b+pow(u,2.0)*C+Delta0/(pow(u,2.0)*C))/(3.0*a);
cout << "D is zero and the roots are: "<< x1 << ", " << x2 << endl;
}else { //D is negative and there is only one solution

 double x1 = (b+pow(u,1.0)*C+Delta0/(pow(u,1.0)*C))/(3.0*a);
cout << "D is negative and the roots is: "<< x1 << endl;
}
	
return 0; 
}