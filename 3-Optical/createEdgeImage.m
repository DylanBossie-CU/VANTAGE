function edgeImage = createEdgeImage(pixels,I)
edgeImage = zeros(size(I));
for i = 1:length(pixels(:,1))
    edgeImage(pixels(i,1),pixels(i,2)) = 1;
end
C = corner(edgeImage);
imshow(edgeImage)
hold on
plot(C(:,1),C(:,2),'r*');
end