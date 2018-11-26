function plotBoundingCentroid(b,j)
    str = num2str(j);
    sz = 600;
    scatter(mean(b(1:end-1,1)),mean(b(1:end-1,2)),sz,'r','+','LineWidth',5);
    text(mean(b(1:end-1,1)),mean(b(1:end-1,2))+200,str,'Color','red',...
        'FontSize',60);
    %plot(b(:,1),b(:,2),'g','LineWidth',3);
end