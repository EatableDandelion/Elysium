clear all;
close all;
clc;

v = [0, -4];
x = [10, 10];
nMasses = 2;
masses = [-20, 0, 100; 20, 0, 100];
vbis = v;
xbis = x;
dt = 0.2;
tmax = 200;
n = tmax/dt;

figure(1)
hold on;
for j = 1:nMasses
	plot(masses(j,1),masses(j,2),'ro');
end


for i = 1:n
	F = [0,0];
	for j = 1:nMasses
		r = sqrt(dot(xbis-masses(j,1:2),xbis-masses(j,1:2)));
		r_hat = (xbis-masses(j,1:2))/r;
		F = F - masses(j,3)*r_hat/(r*r);
	end

	vbis = vbis + dt * F;
    xbis = xbis + dt * vbis;

	plot(xbis(1),xbis(2),'b+');
end

for i = 1:n/20
	
	F = [0,0];

	dx = [0,0];
	dv = [0,0];
	totalW = 0;

	for j = 1:nMasses
		[x_new, v_new, w] = getKeplerNextPosition(x, v, 1, ...
											   masses(j,:), 20*dt);
		totalW = totalW + w;
		dx = dx + (x_new - x)*w;
		dv = dv + (v_new - v);
	end

	v = v + dv;
    x = x + dx/totalW;

	plot(x(1),x(2),'r+');
end

axis equal;
