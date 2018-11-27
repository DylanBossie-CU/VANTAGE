function I_deblurred = DeblurImage(I)
    PSF = fspecial('gaussian',50,2);
    INITPSF = ones(size(PSF));
    I_deblurred = deconvblind(I,INITPSF,5);
    return
end