syms t x(t) y(t) z(t)

tEnd = 400;
tSize = 1000;
dSize = 10000;
%y0 = [50, 2, 2];
y0 = [-9.25, 0.25, 0.25];
tspan = linspace(0, tEnd, dSize);
[t,y] = ode45(@odefun,tspan,y0);
[t2,y2] = ode45(@odefun,linspace(0, tEnd, tSize),y0);
plot3(y(:,1),y(:,2),y(:,3))

x = y(:,1)
%fnn(x, 4, [], 3)
%crp(y2(:,1), 3, 1, 0.5, 'euclidean')
mi(x, 10, 64)
%phasespace(x)



function dydt = odefun(t,yy)
    x = yy(1)
    y = yy(2)
    z = yy(3)
    dydt = [
%        1000-3*x-1000*y^2+10*z^2,
%        y+2*z+x*(y+z*4/3),
%        -2*y+z+x*(-y*4/3+z)
         -y-z,
         x+0.2*y,
         0.2+z*(x-5.7)
    ];
end
