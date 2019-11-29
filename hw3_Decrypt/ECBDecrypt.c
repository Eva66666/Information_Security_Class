#include<stdio.h>
#include<openssl/aes.h>
#include<stdint.h>
#include<malloc.h>
#include<string.h>
unsigned char* ppm_load(char* filename, int* w, int* h)
{
	char header[1024];
	FILE* fp = NULL;
	int line = 0;
	fp = fopen(filename, "rb");
	// 读取图片格式(例如:"P6")
	// 高宽在第二行非注释数据
	while (line < 2) {
		fgets(header, 1024, fp);
		if (header[0] != '#') {
			++line;
		}
	}
	// 读取宽高
	sscanf(header, "%d %d\n", w, h);
	// 获取最大像素值
	fgets(header, 20, fp);
	//
        unsigned char* out_data = (unsigned char*)malloc((*w)*(*h) * 3); 
	fread(out_data,1, (*w)*(*h) * 3 , fp);
	fclose(fp);
        return out_data;
}
void ppm_save(char* filename, unsigned char* data, int w, int h)
{
	FILE* fp;
	char header[20];
	fp = fopen(filename, "wb");
	// 写图片格式、宽高、最大像素值
	fprintf(fp, "P6\n%d %d\n255\n", w, h);
	// 写RGB数据	
	fwrite(data, w*h * 3, 1, fp);
	fclose(fp);
}
int main(int arv, char *argv[]){
    //图片的宽高
    int w,h;
    //得到图片的数据
    unsigned char* pic_data = ppm_load("./ECB_image.ppm",&w,&h);
    //解密后的数据，不够16位补成0
    int len = (w*h*3)%16 == 0 ? (w*h*3)/16 : w*h*3/16+1; 
    unsigned char* decrypt_data = (unsigned char*)malloc(len*16);
    memset(decrypt_data,0,len*16);
    memcpy(decrypt_data,pic_data,w*h*3);
    //ECB模式解密
    unsigned char userkey[16] = { 0x74,0x68,0x69,0x73,0x20,0x69,0x73,0x20,0x6b,0x65,0x79,0x6b,0x65,0x79,0x6b,0x65 };
    AES_KEY key;
    AES_set_decrypt_key(userkey,128,&key);
    for(int i=0;i<len;i++){
	AES_decrypt(&decrypt_data[i*16],&decrypt_data[i*16],&key);        
    }
    //生成解密后的图像
    ppm_save("./ecb_decrypt.ppm",decrypt_data,w,h);
    free(pic_data);
    free(decrypt_data);
    return 0; 
}
