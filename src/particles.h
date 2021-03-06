/* --------------------------------------------------------------------
EXTREME TUXRACER

Copyright (C) 1999-2001 Jasmin F. Patry (Tuxracer)
Copyright (C) 2010 Extreme Tux Racer Team

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.
---------------------------------------------------------------------*/

#ifndef PARTICLES_H
#define PARTICLES_H

#include "bh.h"
#include "mathlib.h"
#include <vector>

// --------------------------------------------------------------------
//					snow for menu screens
// --------------------------------------------------------------------

void init_ui_snow();
void update_ui_snow(float time_step);
void push_ui_snow(const TVector2i& pos);
void draw_ui_snow();

// --------------------------------------------------------------------
//					snow particles during race
// --------------------------------------------------------------------

void update_particles(float time_step);
void clear_particles();
void draw_particles(const CControl *ctrl);
void generate_particles(const CControl *ctrl, double dtime, const TVector3d& pos, double speed);

// --------------------------------------------------------------------
//					snow flakes for short distances
// --------------------------------------------------------------------

struct TFlake {
	TVector3d pt;
	TVector3d vel;
	float size;
	const GLfloat* tex;

	void Draw(const TPlane& lp, const TPlane& rp, bool rotate_flake, float dir_angle) const;
};

struct TFlakeArea {
	std::vector<TFlake> flakes;

	float left;
	float right;
	float bottom;
	float top;
	float front;
	float back;

	float xrange;
	float ytop;
	float yrange;
	float zback;
	float zrange;
	float minSize;
	float maxSize;
	float speed;
	bool  rotate_flake;

	TFlakeArea(
	    std::size_t num_flakes,
	    float xrange_,
	    float ytop_,
	    float yrange_,
	    float zback_,
	    float zrange_,
	    float minSize_,
	    float maxSize_,
	    float speed_,
	    bool  rotate);
	void Draw(const CControl* ctrl) const;
	void Update(float timestep, float xcoeff, float ycoeff, float zcoeff);
};

class CFlakes {
private:
	TVector3d snow_lastpos;
	std::vector<TFlakeArea> areas;
	void MakeSnowFlake(std::size_t ar, std::size_t i);
	void GenerateSnowFlakes(const CControl *ctrl);
	void UpdateAreas(const CControl *ctrl);
public:
	void Init(int grade, const CControl *ctrl);
	void Reset();
	void Update(float timestep, const CControl *ctrl);
	void Draw(const CControl *ctrl) const;
};

// --------------------------------------------------------------------
//					snow clouds for medium distances
// --------------------------------------------------------------------
// intended for future versions

// --------------------------------------------------------------------
//					snow curtains for greater distances
// --------------------------------------------------------------------

#define MAX_CURTAIN_COLS 16
#define MAX_CURTAIN_ROWS 8

struct TCurtainElement {
	TVector3d pt;
	float angle;
	float height;
	float zrandom;
};

struct TCurtain {
	TCurtainElement curtains[MAX_CURTAIN_COLS][MAX_CURTAIN_ROWS];
	int chg[MAX_CURTAIN_ROWS];	// for each row

	unsigned int numCols;
	unsigned int numRows;
	float zdist;
	float size;
	float speed;
	float angledist;
	float startangle;
	float lastangle;
	float minheight;
	int texture;

	TCurtain(
	    int num_rows,
	    float z_dist,
	    float tex_size,
	    float base_speed,
	    float start_angle,
	    float min_height,
	    int dense);
	void SetStartParams(const CControl* ctrl);
	void Draw() const;
	void Update(float timestep, const TVector3d& drift, const CControl* ctrl);

private:
	static void CurtainVec(float angle, float zdist, float &x, float &z);
};

class CCurtain {
private:
	std::vector<TCurtain> curtains;

	void SetStartParams(const CControl *ctrl);
public:
	void Init(const CControl *ctrl);
	void Update(float timestep, const CControl *ctrl);
	void Draw();
	void Reset();
};

// --------------------------------------------------------------------
//					wind
// --------------------------------------------------------------------

struct TWindParams {
	float minSpeed;
	float maxSpeed;
	float minChange;
	float maxChange;

	float minAngle;
	float maxAngle;
	float minAngleChange;
	float maxAngleChange;

	float topSpeed;
	float topProbability;
	float nullProbability;
};

class CWind {
private:
	bool windy;
	float CurrTime;
	TWindParams params;
	// if dest > curr state the modes are 1, otherwise 0
	int SpeedMode;
	int AngleMode;

	float WSpeed;
	float WAngle;
	TVector3d WVector;

	float DestSpeed;
	float DestAngle;
	float WindChange;
	float AngleChange;

	void SetParams(int grade);
	void CalcDestSpeed();
	void CalcDestAngle();
public:
	CWind();

	void Update(float timestep);
	void Init(int wind_id);
	bool Windy() const { return windy; }
	float Angle() const { return WAngle; }
	float Speed() const { return WSpeed; }
	const TVector3d& WindDrift() const { return WVector; }
};

extern CWind Wind;

// --------------------------------------------------------------------
// 			Acess functions
// --------------------------------------------------------------------

void InitSnow(const CControl *ctrl);
void UpdateSnow(float timestep, const CControl *ctrl);
void DrawSnow(const CControl *ctrl);
void InitWind();
void UpdateWind(float timestep);

#endif
