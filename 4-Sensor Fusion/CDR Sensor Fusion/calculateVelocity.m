function velocityOut = calculateVelocity(positions,timestamps)
velocity = zeros(length(positions(:,1))-1,3);
for i = 1:length(positions(:,1))-1
    velocity(i,:) = [positions(i+1,1)-positions(i,1),positions(i+1,2)-...
        positions(i,2),positions(i+1,3)-positions(i,3)];
end
velocityOut = [mean(velocity(:,1)),mean(velocity(:,2)),mean(velocity(:,3))];
end