void zoom(Mat from, Mat to);
void color_zoom(Mat from, Mat to);
void adj_contrast(Mat mat, double ratio);
void set_vmpp2_vangle(float vangle);
float get_vangle();
float get_hangle();
Point get_coord(double xm, double zm, double ym);
void init_pf_coordinate(void);
void write_platform(Mat mat, Scalar lrec, Scalar hrec);
