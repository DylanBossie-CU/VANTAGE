function plotBoundingCentroid(b,j)
    str = num2str(j);
    sz = 600;
    scatter(mean(b(1:end-1,1)),mean(b(1:end-1,2)),sz,'b','+','LineWidth',4);
    text(mean(b(1:end-1,1)),mean(b(1:end-1,2))+200,str,'Color','red',...
        'FontSize',60);
end