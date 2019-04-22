function [R,t] = rigid_transform_3D_function(A, B)
    if nargin ~= 2
	    error("Missing parameters");
    end

    centroid_A = mean(A);
    centroid_B = mean(B);
    disp(num2str(centroid_A));
    disp(num2str(centroid_B));
    N = size(A,1);
    disp(num2str(N));
    H = (A - repmat(centroid_A, N, 1))' * (B - repmat(centroid_B, N, 1));
%{
    A_move=A - repmat(centroid_A, N, 1);
    B_move=B - repmat(centroid_B, N, 1);
    A_norm = sum(A_move.*A_move,2);
    B_norm = sum(B_move.*B_move,2);
    %����߶�ƽ��ֵ
    lam2=A_norm./B_norm;
    lam2=mean(lam2);
    str = (["lam2 = ",num2str(lam2)]);
    disp(str);
    %}
    
    [U,S,V] = svd(H);

    R = V*U';

    if det(R) < 0
        disp("Reflection detected\n");
        V(:,3) = V(:,3)*(-1);
        R = V*U';
    end
  %�����ǳ߶ȵ�ƽ������
    t = -R*centroid_A' + centroid_B';
    %���㿼�ǳ߶Ȳ�ͬ����ת������ƽ������
   %{
    t = -R./(lam2^(0.5))*centroid_A' + centroid_B';
    R = R./(lam2^(0.5));
    %}
    
    disp(R);
    disp(t);
end

