/**
 *    ||          ____  _ __
 * +------+      / __ )(_) /_______________ _____  ___
 * | 0xBC |     / __  / / __/ ___/ ___/ __ `/_  / / _ \
 * +------+    / /_/ / / /_/ /__/ /  / /_/ / / /_/  __/
 *  ||  ||    /_____/_/\__/\___/_/   \__,_/ /___/\___/
 *
 * Crazyflie control firmware
 *
 * Copyright (C) 2011-2012 Bitcraze AB
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, in version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 *
 * stabilizer.h: Stabilizer orchestrator
 */
#ifndef __STABILIZER_TYPES_H__
#define __STABILIZER_TYPES_H__

#include <stdint.h>
#include <stdbool.h>
#include "imu_types.h"

/* Data structure used by the stabilizer subsystem.
 * All have a timestamp to be set when the data is calculated.
 */

/** Attitude in euler angle form */
typedef struct attitude_s {
  uint32_t timestamp;  // Timestamp when the data was computed

  float roll;   // deg
  float pitch;  // deg
  float yaw;    // deg
} attitude_t;

/* x,y,z vector */
struct vec3_s {
  uint32_t timestamp; // Timestamp when the data was computed

  float x;
  float y;
  float z;
};

typedef struct vec3_s point_t;    // m
typedef struct vec3_s velocity_t; // m/s
typedef struct vec3_s acc_t;      // m/s^2

/* Orientation as a quaternion */
typedef struct quaternion_s {
  uint32_t timestamp;

  union {
    struct {
      float q0;
      float q1;
      float q2;
      float q3;
    };
    struct {
      float x;
      float y;
      float z;
      float w;
    };
    float q_arr[4];
  };
} quaternion_t;

typedef struct baro_s {
  float pressure;
  float temperature;
  float asl;
} baro_t;

typedef struct sensorData_s {
  Axis3f acc;
  Axis3f gyro;
  Axis3f mag;
  baro_t baro;
  point_t position;
  quaternion_t quaternion;
  bool valid;
} sensorData_t;

typedef struct state_s {
  attitude_t attitude; // deg
  attitude_t attitudeRate; // rad/s
  point_t position; // m
  velocity_t velocity; // m/s
  acc_t acc;
} state_t;

typedef struct control_s {
  int16_t roll;
  int16_t pitch;
  int16_t yaw;
  float thrust;
} control_t;

typedef enum mode_e {
  modeDisable = 0,
  modeAbs,
  modeVelocity
} mode_t;

typedef struct setpoint_s {
  attitude_t attitude;
  attitude_t attitudeRate; // rad/s
  float thrust;
  point_t position;
  velocity_t velocity;

  struct {
    mode_t x;
    mode_t y;
    mode_t z;
    mode_t roll;
    mode_t pitch;
    mode_t yaw;
  } mode;
  bool enablePosCtrl;
} setpoint_t;

/** Estimate of position */
typedef struct estimate_s {
  uint32_t timestamp; // Timestamp when the data was computed

  point_t position;
} estimate_t;

/** Pose estimate */
typedef struct {
  point_t position;
  attitude_t attitude;
} pose_t;

/** Setpoint for althold */
typedef struct setpointZ_s {
  float z;
  bool isUpdate; // True = small update of setpoint, false = completely new
} setpointZ_t;

// Frequencies to bo used with the RATE_DO_EXECUTE_HZ macro. Do NOT use an arbitrary number.
#define RATE_1000_HZ 1000
#define RATE_500_HZ 500
#define RATE_250_HZ 250
#define RATE_100_HZ 100

#define RATE_MAIN_LOOP RATE_1000_HZ

#define RATE_DO_EXECUTE(RATE_HZ, TICK) ((TICK % (RATE_MAIN_LOOP / RATE_HZ)) == 0)

#endif
