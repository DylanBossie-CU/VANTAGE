function plotEdgeCentroid(b,j)
    sz = 600;
    scatter(mean(b(:,2)),mean(b(:,1)),sz,'r','+','LineWidth',5);
    str = num2str(j);
    %text(mean(b(:,2)),mean(b(:,1))+200,str,'Color','red',...
    %    'FontSize',60);
    plot(b(:,2),b(:,1),'g','LineWidth',3);

end