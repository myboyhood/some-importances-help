% This function finds the optimal Rigid/Euclidean transform in 3D space
% It expects as input a Nx3 matrix of 3D points.
% It returns R, t

% You can verify the correctness of the function by copying and pasting these commands:
%{

R = orth(rand(3,3)); % random rotation matrix

if det(R) < 0
    V(:,3) *= -1;
    R = V*U';
end

t = rand(3,1); % random translation

n = 10; % number of points
A = rand(n,3);
B = R*A' + repmat(t, 1, n);
B = B';

[ret_R, ret_t] = rigid_transform_3D(A, B);

A2 = (ret_R*A') + repmat(ret_t, 1, n)
A2 = A2'

% Find the error
err = A2 - B;
err = err .* err;
err = sum(err(:));
rmse = sqrt(err/n);

disp(sprintf("RMSE: %f", rmse));
disp("If RMSE is near zero, the function is correct!");

%}

% expects row data

camera = importdata('fcamera.txt');
fcamera = fopen('redball_camera_xyz.txt','wt');
for i = 1:81
    fprintf(fcamera,'%g ',camera(i,2));
    fprintf(fcamera,'%g ',camera(i,3));
    fprintf(fcamera,'%g ',camera(i,4));
    fprintf(fcamera,'\n');
end
stacam = fclose(fcamera);
if (stacam == 0)
    disp('successfully close redball_camera_xyz.txt');
end

marker = importdata('fselect.txt');
fmarker = fopen('redball_marker_xyz.txt','wt');
for j = 1:81
    fprintf(fmarker,'%g ',marker(j,2));
    fprintf(fmarker,'%g ',marker(j,3));
    fprintf(fmarker,'%g ',marker(j,4));
    fprintf(fmarker,'\n');
end
stamark = fclose(fmarker);
if (stamark == 0)
    disp('successfully close redball_marker_xyz.txt');
end

A = importdata('redball_camera_xyz.txt');
B = importdata('redball_marker_xyz.txt');

[ret_R, ret_t] = rigid_transform_3D_function(A, B);

n = 81;
A2 = (ret_R*A') + repmat(ret_t, 1, n);
A2 = A2';

% Find the error
err = A2 - B;
err = err .* err;
err = sum(err(:));
rmse = sqrt(err/n);

%作图
ball_0 = [0.319587,-0.379767,2.08139];
ball_1 = [0.211617, -0.46137,1.94429];
ball_2 = [0.176,  -0.468385,  1.9984];
ball_3 = [0.324202,  -0.179114,  1.66077];
ball_convert = ret_R*ball_3' + ret_t;

plot3(ball_3(1,1),ball_3(1,2),ball_3(1,3),'ms');
grid on;
hold on;
plot3(ball_convert(1,1),ball_convert(2,1),ball_convert(3,1),'rs');
hold on;
plot3(0.545,-0.337,0.083,'bs');%球真值坐标



plot3(A(:,1),A(:,2),A(:,3),'-ms')
plot3(A2(:,1),A2(:,2),A2(:,3),'-r*');
xlabel('x');
ylabel('y');
zlabel('z');
hold on;
plot3(B(:,1),B(:,2),B(:,3),'-bo');

str = (['RMSE = ',num2str(rmse)]);
disp(str);
disp("If RMSE is near zero, the function is correct!");


