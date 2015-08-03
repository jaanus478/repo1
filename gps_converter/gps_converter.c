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
	double lat;
	double lon;
} gps_pos;

gps_pos cartesian_to_gps(double x, double y);
/*convert cartesian coordinates to gps coordinates*/

double calc_dist_degrees(gps_pos* start, gps_pos* end);
/*Calculate a distance between two positions given as doubles*/
/*Result is returned in degrees*/

double calc_dist_meters(gps_pos* start, gps_pos* end);
/*Result is returned in meters*/

double calc_start_course(gps_pos* start, gps_pos* end);
/*Calculate starting course given start and end positions as doubles*/
/*Result is returned as degrees*/

/*UTILITY FUNCTIONS*/
double calc_end_pos_lat(gps_pos* start, double start_course, double dist);
/*Calculate end position latitude*/

double d_sin(double deg);
/*convert argument deg to radians and compute sine func*/

double d_cos(double deg);
/*convert argument deg to radians and compute cosine func*/

double rad_to_deg(double rad);
double deg_to_rad(double deg);

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


	gps_pos start;
	gps_pos end;

	start.lat = 59.12;
	start.lon = 22.43;
	end.lat = 59.12;
	end.lon = 22.43;

	gps_pos road_start;
	gps_pos road_end;

	road_start.lat = 59.394442;
	road_start.lon = 24.673424;

	road_end.lat = 59.392792;
	road_end.lon = 24.670498;

	printf("road width = %lf\n", calc_dist_meters(&road_start, &road_end));

	double dist = calc_dist_meters(&start, &end);
	printf("distance is: %f\n", dist);

	gps_pos test = cartesian_to_gps(3, 2);

	start.lat = ORIGIN_LAT;
	start.lon = ORIGIN_LON;

	printf("original latitude: %f\n", start.lat);
	printf("original longitude: %f\n", start.lon);

	printf("the calculated latitude is: %f\n", test.lat);
	printf("the calculated longitude is: %f\n", test.lon);
	





	printf("distance calculated again is: %f\n", calc_dist_meters(&start, &test));


	//fflush(stdout); 
	return 0;

}


/*******************/
/*calc_dist_degrees*/
/*******************/

double calc_dist_degrees(gps_pos* start, gps_pos* end)
{
	
	double val, dist;

	val = d_sin(start->lat) * d_sin(end->lat) + d_cos(start->lat) * d_cos(end->lat) * d_cos(start->lon - end->lon);
	dist = rad_to_deg (acos(val));
	/*
	printf("start gps latitude is: %f\n", start->lat);
	printf("start gps latitude is: %f\n", start->lon);

	printf("end gps latitude is: %f\n", end->lat);
	printf("end gps latitude is: %f\n", end->lon);
	*/
	return dist;
}


/*******************/
/*calc_dist_degrees*/
/*******************/

double calc_dist_meters(gps_pos* start, gps_pos* end)
{
	return calc_dist_degrees(start, end) * 1852 * 60;

}

/*******************/
/*calc_start_course*/
/*******************/

double calc_start_course(gps_pos* start, gps_pos* end){
	double num = d_sin(end->lat) - d_sin(start->lat) * d_cos(calc_dist_degrees(start, end));
	double denom = d_cos(start->lat) * d_sin(calc_dist_degrees(start, end));
	double res = num / denom;
	double course = rad_to_deg(acos(res));
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

double calc_end_pos_lat(gps_pos* start, double start_course, double dist){
	double dist_degrees = (dist / 1852.0) / 60.0;
	double left_exp = d_cos(start_course) * d_cos(start->lat) * d_sin(dist_degrees);
	double right_exp = d_sin(start->lat) * d_cos(dist_degrees) * d_cos(dist_degrees);
	return rad_to_deg(asin(left_exp + right_exp));
}


/**************/
/*calc_end_pos*/
/**************/

gps_pos calc_end_pos(gps_pos* start, double start_course, double dist){

	gps_pos *end = (gps_pos*)malloc(sizeof(gps_pos));

	double dist_degrees = (dist / 1852.0) / 60.0;
	double end_pos_lat = calc_end_pos_lat(start, start_course, dist);
	double num = d_cos(dist_degrees) - d_sin(start->lat) * d_sin(end_pos_lat);
	double denom = d_cos(start->lat) * d_cos(end_pos_lat);
	
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

gps_pos cartesian_to_gps(double x, double y){

	double dist = sqrt(pow(x, 2) + pow(y, 2));

	printf("long float distance on cartesian axes: %lf\n", dist);
	printf("float distance on cartesian axes: %f\n", dist);
	
	double start_course = 90.0 - rad_to_deg(asin(y/dist));
	
	gps_pos* start = (gps_pos*)malloc(sizeof(gps_pos));

	start->lat = ORIGIN_LAT;
	start->lon = ORIGIN_LON;

	return calc_end_pos(start, start_course, dist);
}





double d_sin(double deg) { return (sin(deg_to_rad (deg))); }
double d_cos(double deg) { return (cos(deg_to_rad(deg))); }

double rad_to_deg(double rad) { return rad * 180 / PI; }
double deg_to_rad(double deg) { return deg * PI / 180; }
