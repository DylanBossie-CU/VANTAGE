function [ptNew] = ptRot(ptCloud,d_tr,plotting)
    d_t = d_tr(1:3);
    d_r = d_tr(4:6);
    R = eul2rotm(d_r);
    x = ptCloud.Location(:,1);
    y = ptCloud.Location(:,2);
    z = ptCloud.Location(:,3);
    tmp = [x,y,z]*R;
    ptNew = pointCloud([tmp(:,1) + d_t(1),tmp(:,2) + d_t(2),tmp(:,3) + d_t(3)]);

    if plotting
        figure
        pcshow(ptNew)
        hold on
        plot3(0,0,0,'k*')
    end
end