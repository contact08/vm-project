
enum {
	CUI_INT,
	CUI_UINT,
	CUI_FLOAT,
	CUI_DOUBLE,
	CUI_INDEX,
};

typedef struct {
	const char	*name;
	int	type;
	void	*ptr;
	double	step;
	double	max;
	double	min;
} CUI_VAR;

/*
example:
static int	cm = 100;
CUI_VAR	cui_var = {
	"distance in cm",
	CUI_UINT,
	(void*)(&cm),
	1,
	1000,	
	50,
};
*/

typedef struct {
	Scalar	lrec;	// lower rectagle (GROUND)
	Scalar	hrec;	// top suface of platform
	double	altitude;	// ground to top surface
	double	ground;		// ground position
	Mat	original;	// not used now!
	// 2D platform coordinate in pixel
	Point	pfcoord[8];
	// 3D distance in m (for 3d rotaion)
	double	z_in_m[8];
	Scalar	color;
} ARBOX;

ARBOX *new_arbox(void);
void destroy_arbox(ARBOX *ptr);
void set_arbox(ARBOX *ptr, Scalar rec, double altitude, double ground);
void set_rolling(int degree);
void set_pitching(int degree);
void init_pf_coordinate(ARBOX *ptr);
void write_arbox(Mat mat, ARBOX *ptr);
void erase_arbox(Mat mat, ARBOX *ptr);

void zoom(Mat from, Mat to);
void color_zoom(Mat from, Mat to);
void adj_contrast(Mat mat, double ratio);
void set_vmpp2_vangle(float vangle);
float get_vangle();
float get_hangle();
Point get_coord(double xm, double zm, double ym);
void inc_cui_var(CUI_VAR *var);
void dec_cui_var(CUI_VAR *var);

void rotate_by_x(float *x, float *y, float *z, int degree);
void rotate_by_y(float *x, float *y, float *z, int degree);
void rotate_by_z(float *x, float *y, float *z, int degree);
