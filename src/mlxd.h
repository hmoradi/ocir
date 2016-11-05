/**@file GridEyeReader.h
 *
 * @date June 30, 2016
 * @author Hessam Mohammadmoradi <hmoradi@cs.uh.edu>
 */

#ifndef MLXD_H_
#define MLXD_H_
#include "buffer.h"

using namespace std;

class MLXD {
public:
	unsigned char confreg_lsb;
	unsigned char confreg_msb;
	int res = 1;
	float temperatures[64];
	unsigned short temperaturesInt[64];


	unsigned char EEPROM[256];
	unsigned char ir_pixels[128];
	int Acommon ;
    int Acp;
    int ksta;
    float ta;
     /* Compensation pixel individual offset coefficients */
    int acp =0;
    /* Individual Ta dependence (slope) of the compensation pixel offset */
    int bcp=0;
    int vcp;
    float alpha;
    float vir_compensated;
    /* Sensitivity coefficient of the compensation pixel */
    int alphacp=0;
    /* Thermal Gradient Coefficient */
    int tgc=0;
    /* Scaling coefficient for slope of IR pixels offset */
    int bi_scale=0;
    /* Common sensitivity coefficient of IR pixels */
    int alpha0=0;
    /* Scaling coefficient for common sensitivity */
    int alpha0_scale=0;
    /* Scaling coefficient for individual sensitivity */
    int delta_alpha_scale=0;
    /* Emissivity */
    float epsilon=0.0;
    Buffer buffer_;
	int mlx90620_init ();
	int mlx90620_read_eeprom ();
	int mlx90620_write_config (unsigned char *lsb, unsigned char *msb);
	int mlx90620_read_config (unsigned char *lsb, unsigned char *msb);
	int mlx90620_write_trim (unsigned char t);
	char mlx90620_read_trim ();
	int mlx90620_por ();
	int mlx90620_set_refresh_hz (int hz);
	int mlx90620_ptat ();
	int mlx90620_cp ();
	float mlx90620_ta ();
	int mlx90620_ir_read ();
	void readFrameLive(int& frameN_read);
	void init();
	int** next_frame();
    void read_frameMLX_from_file(std::ifstream* infile);

};

#endif /* MLXD_H_ */