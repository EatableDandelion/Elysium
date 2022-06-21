function [x_new, v_new, w] = getKeplerNextPosition(x, v, m, masses, dt)
%https://space.stackexchange.com/questions/1904/how-to-programmatically-calculate-orbital-elements-using-position-velocity-vecto
	mu = (m+masses(3));
	x = x - masses(1:2);

	r = sqrt(dot(x,x));

	w = mu/(r*r);

	r_hat = x/r;
	t_hat = [-r_hat(2), r_hat(1)];

	Vr = dot(v, r_hat);
	Vt = dot(v, t_hat);

	orbitDirection = sign(Vt);

	Vt = abs(Vt);

	dtheta = Vt * dt / r;

	p = (r*Vt)^2/mu;
	V0 = sqrt(mu/p);
	e = sqrt((Vt/V0-1)^2 + (Vr/V0)^2);

%	e_vec = ((dot(v,v)-mu/r)*x-dot(x,v)*v)/mu;

%	e_vec = (x(1)*v(2)-x(2)*v(1))*[v(2) -v(1)]/mu-x/r;

%	e = sqrt(dot(e_vec, e_vec));

	theta = atan2(Vr/V0, Vt/V0-1);
	theta0 = atan2(x(2),x(1));

	theta_new = theta + dtheta;
	r = p/(1+e*cos(theta_new));

	theta_world = theta0 + orbitDirection*dtheta;
	x_new(1) = r*cos(theta_world);
	x_new(2) = r*sin(theta_world);

	Vr = sqrt(mu/p) * e * sin(theta_new);
	Vt = orbitDirection * sqrt(mu/p) * (1 + e * cos(theta_new));

	r_hat = x_new/r;
	t_hat = [-r_hat(2), r_hat(1)];

	v_new = Vr * r_hat + Vt * t_hat;

	x_new = x_new + masses(1:2);


	%semi-major axis
	a = p/(1-e*e);
	b = a*sqrt(1-e*e);

	%Eccentric Anomaly
%	E = atan2(sqrt(1-e*e)*sin(theta), e + cos(theta));


	th = 0;
	dth = 2*3.14/19;
	theta0 = theta0 - orbitDirection*theta - 3.141593;
	x0 = zeros(20,1);
	y0 = zeros(20,1);
	for k = 1 : 20
		x0(k) = a*(e+cos(th))*cos(theta0) - b*sin(th)*sin(theta0);
		y0(k) = a*(e+cos(th))*sin(theta0) + b*sin(th)*cos(theta0);	
		x0(k) = x0(k) + masses(1);
		y0(k) = y0(k) + masses(2);
		th = th + dth;
	end
	plot(x0,y0,'r+');

	%Mean anomaly
	%M = E - e * sin(theta);
	%Mean motion
	%t = a*sqrt(a/mu)*M;


