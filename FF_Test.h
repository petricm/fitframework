/**
 * @file	FFTest.h
 * @date	Aug 25, 2011
 * @author	petric
 * @brief	brief description
 *
 * long description
 */

#ifndef FFTEST_H_
#define FFTEST_H_

// Fit Framework
#include "FitFramework.h"
#include "FF_Util.h"
// system
#include "time.h"
// Utilities
#include "mp_utility.h"

namespace FFTest {

void PrintTimestamp();

void TestingResonancesPDF();

void TestingKKModels();

void TestingAngularComputation();

void TestingSphericHarmonic();

void TestingHelicityAnglesPDF(int j1, int j2);

void TestingMassHelicityAnglesPDF_CPV(bool draw_projections, ACCEPTANCE_FUNCTION acceptance, int mode);

void TestingPhysicsParameterComputation(ACCEPTANCE_FUNCTION acceptance, const std::string& config_file);

void ScanResonanceAmplitudes();

void TestingPHSPProjection();

}

#endif /* FFTEST_H_ */
