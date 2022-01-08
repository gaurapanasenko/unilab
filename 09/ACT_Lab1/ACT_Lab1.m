disp("-------- Lab 1 -------")
disp("--- Task 1")
A = [3.25 -1.07 2.34; 10.10 0.25 -4.78; 5.04 -7.79 3.31]

disp("--- Task 2")
B=rand(3)

disp("--- Task 3")
T=rand(3)
one=A+10*B
two=A*T
three=B'
four=A.*B
sorted=sort(A)
min_elem=min(reshape(A, 9, 1))
max_elem=max(reshape(A, 9, 1))
d=det(A)

disp("--- Task 4")
C=zeros(3,1)
C(2) = 5.71
C(3) = -3.61

disp("--- Task 5")
sym X
task5 = A^-1*C

disp("--- Task 6")
D=sin(A)+B^(3/5)

disp("--- Task 7")
a=[3.2 2.8 -1.4]
b=[0.6 3.2 -4.8]
ab = a.*b
axb = cross(a, b)
na = norm(a)

disp("--- Task 8")
x = 0:0.5:4*pi
y = exp(-x.^2)
z = atan(x).^(1/2)
plot(x, y, x, z)
hold on

disp("--- Task 9")
figure
subplot(1,2,1)
plot(x, y)
subplot(1,2,2)
plot(x, z)
hold on

disp("--- Task 10")
figure
x=linspace(1,2,10)
y=x
[xx, yy] = meshgrid(x, y)
mesh(log(xx.^2+yy.^2-xx.*yy))
hold off

disp("-------- Lab 1.2 -------")
disp("--- Task 1")
syms x y z
sys = [x+y-z==36, x+z+y==13, y+z-x==7]
[A,B] = equationsToMatrix(sys, [x, y, z])
X = linsolve(A, B)

disp("--- Task 2")
y1 = solve(-3*x^3)
y2 = solve(x^4-2*x^2+10)

disp("--- Task 3")
syms x
f = cos(x)^(1/(x*sin(x)))
l = limit(f, x, 0)

disp("--- Task 4")
syms x y
Dy = diff(log(sqrt(exp(2*x)+1)),x)
eq = x^2*y*2+2*log(x*y) == 4
sol = solve(eq, y)
diff(sol, x)

disp("--- Task 5")
syms x
DDy = diff((x+1)/sqrt(1-x-x^2), x, 2)

disp("--- Task 6")
syms x
fun = @(x)((cos(2.*x)+sin(x).^2)./sin(3.*x))
integral(fun, pi/4, pi/7)

disp("--- Task 7")
syms x y
eval(int(int(x^2*exp(x+sin(y))*cos(y), x, 1, 2), y, 0, 1))

disp("--- Task 8")
int(log(x)/x, x)

disp("--- Task 9")
syms x y(x);
eq = x^2*diff(y,x,2)+3*x*diff(y,x)+y==1/x;
Dy = diff(y,x);
sol(x) = dsolve(eq, [y(1) == 1, Dy(1) == 0])
x = 1:0.1:10;
figure
plot(x, sol(x));

disp("--- Task 10")
syms x y(x);
eq = diff(y, x) == exp(x + y) + exp(x - y)
dsolve(eq)