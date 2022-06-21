clear all;
close all;
clc;

m = [2000; 0.1; 20; 1; 1; 10; 0.1];

n = size(m);
x = zeros(n, 2);
x_old = zeros(n, 2);
v = zeros(n, 2);
F = zeros(n, 2);
trail = zeros(n, 2, 50);
trail(:) = NaN;
G = 1;
dt = 0.2;
mask = zeros(n,1);
mask(:) = 1;

x(1,1) = 0;
x(2,1) = 15;
x(3,1) = 30;
x(4,1) = 38;
x(5,1) = 40;
x(6,1) = 80;
x(7,1) = 85;

for i = 2 : n
	dx = x(1,:) - x(i,:);
	a = sqrt(dot(dx,dx));
	velocity = sqrt(G*(m(1)+m(i))/a);
	v(i,1) =  velocity * dx(2) / a;
	v(i,2) = -velocity * dx(1) / a;
	v(1,:) = v(1,:) - v(i,:)*m(i)/m(1);
end

v(2,:) = v(2,:)/2;
%v(2,:) = [0 15];
%dx = x(6,:) - x(7,:);
%v(7,2) = v(6,2) + sqrt(G*m(6)/sqrt(dot(dx,dx)));

figure(1)
hold on
axis equal;
for i = 1 : n
	plot(x(i,1),x(i,2),'bo');
end

xk = x(2,:);
vk = v(2,:);

rmax = 1.5*max(abs(x(1:n,1)));

x_old = x;
for i = 1 : n
	x(i,:) = x(i,:) + v(i,:)*dt;
end

for t = 1 : 500 
	for i = 1 : n
		for j = i+1 : n
			if(mask(i)*mask(j) == 0)
				continue;
			end
			dx = x(i,:) - x(j,:);
			a = sqrt(dot(dx,dx));

			F(i,:) = F(i,:) - G * m(i) * m(j) * dx / a^3;
			F(j,:) = F(j,:) + G * m(i) * m(j) * dx / a^3;
		end
	end

	hold off;
	cog = [0 0];
	mtot = 0;
	for i = 1 : n
		if(mask(i) == 0)
			continue;
		end

%		v(i,:) = v(i,:) + F(i,:) * dt / m(i);
		x_new = 2*x(i,:)-x_old(i,:)+F(i,:)*dt^2/m(i);
		v(i,:) = (x_new - x(i,:))/dt; 
		x_old(i,:) = x(i,:);
		x(i,:) = x_new;
		F(i,:) = 0;

		if(dot(x(i,:),x(i,:)) > 300*300) 
			mask(i) = 0;
		end

		cog = cog + m(i) * x(i,:);
		mtot = mtot + m(i);


		for k = 1 : size(trail,3)-1
			trail(i,:,k) = trail(i,:,k+1);
		end
		trail(i,:,end) = x(i,:);

		plot(trail(i,1,:),trail(i,2,:),'b');
		hold on;
%		plot(x(i,1),x(i,2),'ko');
		quiver(x(i,1),x(i,2),v(i,1)*0.05,v(i,2)*0.05);
	end
	cog = cog / mtot;
%	xlim([-rmax+cog(1) rmax+cog(1)]);
	axis equal

%	if(mod(t,10) == 1)
		totalW = 0;
		dx = [0 0];
		for j = 1:1
			if(j == 2) 
				continue;
			end
			[x_new,v_new,w]=getKeplerNextPosition(x(2,:),v(2,:),m(2),...
												 [x(j,:) m(j)],50*dt);
			totalW = totalW + w;
			dx = dx + (x_new - x(2,:))*w;
		end
		xk = x(2,:) + dx/totalW;
%	end

	plot(xk(1),xk(2),'ro');


		pause(0.01)
end

