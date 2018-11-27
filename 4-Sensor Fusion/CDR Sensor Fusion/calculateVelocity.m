function velocityOut = calculateVelocity(positions,timestamps)
velocity = zeros(length(positions(:,1))-1,3);
for i = 1:length(positions(:,1))-1
    velocity(i,:) = (positions(i+1,:)-positions(i,:))./timestamps(i);
end
velocityOut = [mean(velocity(:,1)),mean(velocity(:,2)),mean(velocity(:,3))];
end