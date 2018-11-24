function I_deblurred = DeblurImage(I)
    radius = 1;
    PSF = fspecial('disk',radius);
    I_deblurred = deconvwnr(I,PSF,0.05);
    imshow(I_deblurred)
    return
end