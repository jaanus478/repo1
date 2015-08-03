#include <stdio.h>
#include <math.h>       /* sin, cos, etc */

#define PI 3.14159265

/*random position (ProLab) defined as origin*/
#define ORIGIN_LAT 59.39453626;
#define ORIGIN_LON 24.67179717;

/*Cartesian y and x axis directions*/
#define NORTH = 0;
#define EAST = 90;
/*In following the cartesian axis y always points to North and x always to East*/


typedef struct gps_pos
{
	float lat;
	float lon;
} gps_pos;

gps_pos cartesian_to_gps(float x, float y);
/*convert cartesian coordinates to gps coordinates*/

float calc_dist_degrees(gps_pos* start, gps_pos* end);
/*Calculate a distance between two positions given as floats*/
/*Result is returned as degrees*/

float calc_start_course(gps_pos* start, gps_pos* end);
/*Calculate starting course given start and end positions as floats*/
/*Result is returned as degrees*/

float calc_end_pos_lat(gps_pos* start, float start_course, float dist);
/*Calculate end position latitude*/

float d_sin(float deg);
/*convert argument deg to radians and compute sine func*/

float d_cos(float deg);
/*convert argument deg to radians and compute cosine func*/

float rad_to_deg(float rad);
float deg_to_rad(float deg);

int main(int argc, char *argv[])
{
	FILE *f = fopen("coordinates.txt", "r");

	char * line = NULL;
	size_t len = 0;
	int read;



	if (f == NULL){
		printf("File does not exist!");
		return 0;
	}

	while ((read = getline(&line, &len, f)) != -1) {
		printf("Retrieved line of length %zu :\n", read);
		printf("%s", line);
	}
	//some comment



	printf("UNIT TESTS:\n");


	float a = 30;

	printf("sine of 30: %f\n", d_sin(a));


	gps_pos start;
	gps_pos end;

	start.lat = 59.12;
	start.lon = 22.43;
	end.lat = 59.12;
	end.lon = 22.43;

	float dist = calc_dist_degrees(&start, &end);
	printf("distance is: %f\n", dist);


	gps_pos test = cartesian_to_gps(3, 2);

	printf("the calculated latitude is: %f\n", test.lat);
	printf("the calculated longitude is: %f\n", test.lon);
	
	
	start.lat = ORIGIN_LAT;
	start.lon = ORIGIN_LON;


	printf("distance calculated again is: %f\n", calc_dist_degrees(&start, &test));

	//fflush(stdout); 
	return 0;

}


/*******************/
/*calc_dist_degrees*/
/*******************/

float calc_dist_degrees(gps_pos* start, gps_pos* end)
{
	
	float val, dist;

	val = d_sin(start->lat) * d_sin(end->lat) + d_cos(start->lat) * d_cos(end->lat) * d_cos(start->lon - end->lon);
	dist = rad_to_deg (acos(val));
	
	printf("start gps latitude is: %f\n", start->lat);
	printf("start gps latitude is: %f\n", start->lon);

	printf("end gps latitude is: %f\n", end->lat);
	printf("end gps latitude is: %f\n", end->lon);

	return dist;
}

/*******************/
/*calc_start_course*/
/*******************/

float calc_start_course(gps_pos* start, gps_pos* end){
	float num = d_sin(end->lat) - d_sin(start->lat) * d_cos(calc_dist_degrees(start, end));
	float denom = d_cos(start->lat) * d_sin(calc_dist_degrees(start, end));
	float res = num / denom;
	float course = rad_to_deg(acos(res));
	if (start->lon > end->lon)
	{
		return 360 - course;
	}
	else
	{
		return course;
	}
}


/******************/
/*calc_end_pos_lat*/
/******************/

float calc_end_pos_lat(gps_pos* start, float start_course, float dist){
	float dist_degrees = (dist / 1852.0) / 60.0;
	float left_exp = d_cos(start_course) * d_cos(start->lat) * d_sin(dist_degrees);
	float right_exp = d_sin(start->lat) * d_cos(dist_degrees) * d_cos(dist_degrees);
	return rad_to_deg(asin(left_exp + right_exp));
}


/**************/
/*calc_end_pos*/
/**************/

gps_pos calc_end_pos(gps_pos* start, float start_course, float dist){

	gps_pos *end = (gps_pos*)malloc(sizeof(gps_pos));

	float dist_degrees = (dist / 1852.0) / 60.0;
	float end_pos_lat = calc_end_pos_lat(start, start_course, dist);
	float num = d_cos(dist_degrees) - d_sin(start->lat) * d_sin(end_pos_lat);
	float denom = d_cos(start->lat) * d_cos(end_pos_lat);
	
	end->lat = end_pos_lat;


	if (start_course > 180)
	{
		end->lon = start->lon - rad_to_deg(acos(num / denom));
	}
	else
	{
		end->lon = start->lon + rad_to_deg(acos(num / denom));
	}
	return *end;
}

/******************/
/*cartesian_to_gps*/
/******************/

gps_pos cartesian_to_gps(float x, float y){

	float dist = sqrt(pow(x, 2) + pow(y, 2));
	
	float start_course = 90.0 - rad_to_deg(asin(y/dist));
	
	gps_pos* start = (gps_pos*)malloc(sizeof(gps_pos));

	start->lat = ORIGIN_LAT;
	start->lon = ORIGIN_LON;

	return calc_end_pos(start, start_course, dist);
}





float d_sin(float deg) { return (sin(deg_to_rad (deg))); }
float d_cos(float deg) { return (cos(deg_to_rad(deg))); }

float rad_to_deg(float rad) { return rad * 180 / PI; }
float deg_to_rad(float deg) { return deg * PI / 180; }
