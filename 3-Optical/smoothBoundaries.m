function smoothBoundary = smoothBoundaries(boundaries,grayImage)

for i = 1:length(boundaries)
    firstBoundary = boundaries{i};
    % Get the x and y coordinates.
    x = firstBoundary(:, 2);
    y = firstBoundary(:, 1);

    % Now smooth with a Savitzky-Golay sliding polynomial filter
    windowWidth = 45;
    polynomialOrder = 4;
    smoothX = sgolayfilt(x, polynomialOrder, windowWidth);
    smoothY = sgolayfilt(y, polynomialOrder, windowWidth);
    % First, display the original gray scale image.
    %{
    subplot(2, 3, 5);
    imshow(grayImage, []);
    axis image; % Make sure image is not artificially stretched because of screen's aspect ratio.
    hold on;
    plot(smoothX, smoothY, 'r-', 'LineWidth', 2);
    grid on;
    title('Smoothed Boundary');
    %}
    smoothBoundary{i} = [smoothX, smoothY];
end
end