/**
 * @file	libComplexPDF.h
 * @date	Nov 9, 2013
 * @author	mprim, mpetric
 * @brief	brief description
 *
 * long description
 */

#ifndef LIBCOMPLEXPDF_H_
#define LIBCOMPLEXPDF_H_

#include <complex>
#include <vector>
#include <tuple>
#include <unordered_map>

namespace ComplexPDF {
    
    /**
     * @brief Utility method to calculate the daugther particle momentum in the resonance system
     * @param m resonance mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daugther
     * @return daughter particle momentum
     */
    double CalculateMomentum(double m, const double m1, const double m2);
    
    /**
     * @brief Computes whether a data point is inside the physicaly allowed region of a Dalitz plane
     * @param m12 invariant mass m_12
     * @param m23 invariant mass m_23
     * @param M nominal mass of mother
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @return true if allowed, false if not
     */
    bool InsideDalitzLimits(const double m12, const double m23, const double M, const double m1, const double m2, const double m3);
    
    /**
     * @brief A sine like treshold function for efficiency parametrization at the edge of the Dalitz plot
     * @param v runnig varible mass_12, etc..
     * @param Ec value of efficiency at exactly the edgle of the Dalitz space
     * @param Eth value of the treshold when the function swiches from constant to sine
     * @param v_max maximal value of v -> edge of Dalitz plot
     */
    double CLEOThresholdFunction(const double v, const double Ec, const double Eth, const double v_max);
    
    /**
     * @brief Utility method to calculate the boundaries in the Dalitz plane
     * @param m resonance mass m_12
     * @param M resonance mother mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @param m23_max_2 result for m_23_max^2 (upper limit in Dalitz plane)
     * @param m23_min_2 result for m_23_min^2 (lower limit in Dalitz plane)
     */
    void DalitzLimits(const double m, const double M, const double m1, const double m2, const double m3, double &m23_max_2, double &m23_min_2);
    
    /**
     * @brief Currently supported angular formalisms for Dalitz angular term
     */
    enum ANGULAR_FORMALISM {
        HELICITY,//!< HELICITY
        ZEMACH   //!< ZEMACH
    };
    
    /**
     * @brief To indice in which combination  is used for resonance and which for the angular part e.g. B0-> (AB) C
     */
    enum COMBINATION {
        AB, //!< B0 -> (AB) C
        BC, //!< B0 -> A (BC)
        CA  //!< B0 -> (CA) B
    };
    
    /**
     * @brief Calculate the angular distribution for a 3 body decay e.g. X -> (12)3 -> 123
     * @param m12 invariant mass m_12
     * @param m23 invariant mass m_23
     * @param J spin of resonance
     * @param m nominal resonance mass m_12
     * @param M nominal mass of mother
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @param formalism ANGULAR_FORMALISM used in calculation
     * @return Angular distribution factor
     */
    double DalitzAngularDistribution(const double m12, const double m23, const unsigned int J, const double m,
                                     const double M, const double m1, const double m2, const double m3, ANGULAR_FORMALISM formalism = ComplexPDF::HELICITY);
    
    /**
     * @brief Just returns a constant amplitude for flat phase space model
     * @param m12 invariant mass m_12 (parameter ignored)
     * @param m23 invariant mass m_23 (parameter ignored)
     * @return Constant amplitude: polar(1.0,0.0)
     */
    std::complex<double> DalitzPhaseSpaceAmplitude(const double m12, const double m23);
    
    /**
     * @brief Cubic two dimensional polynomial for background and efficiency parameterization
     * @param m12 invariant mass m_12
     * @param m23 invariant mass m_23
     * @param E_x Param for x term
     * @param E_y Param for y term
     * @param E_xx Param for x*x term
     * @param E_xy Param for x*y term
     * @param E_yy Param for y*y term
     * @param E_xxx Param for x*x*x term
     * @param E_xxy Param for x*x*y term
     * @param E_xyy Param for x*y*y term
     * @param E_yyy Param for y*y*y term
     * @param m12_offset Possible offset to shift m12 in Dalitz plane
     * @param m23_offset Possible offset to shift m23 in Dalitz plane
     * @return Polynomial at (m12,m23)
     */
    double DalitzCubicPolynomial(const double m12, const double m23,
                                 const double E_x, const double E_y,
                                 const double E_xx, const double E_xy, const double E_yy,
                                 const double E_xxx, const double E_xxy, const double E_xyy, const double E_yyy,
                                 const double m12_offset = 0.0, const double m23_offset = 0.0);
    
    /**
     * @brief Explicitly symmetric cubic two dimensional polynomial for background and efficiency parameterization
     * @param m12 invariant mass m_12
     * @param m23 invariant mass m_23
     * @param E_1 Param of (x+y) term
     * @param E_2 Param of (x*x+y*y) term
     * @param E_3 Param of (x*x*x+y*y*y) term
     * @param E_xy Param of x*y term
     * @param E_xyn Param of (x*y*y + x*x*y) term
     * @param m12_offset Possible offset to shift m12 in Dalitz plane
     * @param m23_offset Possible offset to shift m23 in Dalitz plane
     * @return Polynomial at (m12,m23)
     */
    double DalitzCubicPolynomialSymmetric(const double m12, const double m23,
                                          const double E_1, const double E_2, const double E_3,
                                          const double E_xy, const double E_xyn,
                                          const double m12_offset = 0.0, const double m23_offset = 0.0);
    
    /**
     * @brief Compute the Blatt-Weisskopf form factors
     * @param q momentum in the resonance system at mass m
     * @param q_J momentum in the resonance system at mass m_J (m_J is isual the nominal mass of the resonance)
     * @param r interaction radius
     * @param J spin
     * @return Blatt-Weisskopf form factor
     */
    double BlattWeisskopfFormFactor(const double q, const double q_J, const double r, const unsigned int J);
    
    /**
     * @brief Utility to calculate the phase space factor for one dimension of the Dalitz plane
     * @param m resonance mass m_12
     * @param M resonance mother mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @return Phase space factor
     */
    double PhaseSpaceFactor(const double m, const double M, const double m1, const double m2, const double m3);
    
    /**
     * @brief Projection of the three body phase space Dalitz plane onto one dimension
     *
     * This projection is given by P(m12) = INTEGRAL[m23min^2(m12),m23max^2(m12)] dm23^2 and can be solved analytically
     * @param m12 resonance mass m_12, which is the projection dimension
     * @param M reosnance mother mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @return Phase space amplitude at m12
     */
    double ThreeBodyPhaseSpaceProjection(const double m12, const double M, const double m1, const double m2, const double m3);
    
    /**
     * @brief A threshold function that can describe e.g. the combinatoric background for the KK inv. mass
     *
     * The function is given by f(m) = ArcTan(Sqrt((m - min)/a))
     * @param m mass
     * @param a parameter
     * @param m_min minimal mass (e.g. KK threshold)
     * @return Function value at m
     */
    double TwoBodyThresholdFunction(const double m, const double a, const double m_min);
    
    typedef std::tuple<unsigned int, int, double, double> tuple_spheric_harmonic;
    extern std::unordered_map<tuple_spheric_harmonic, std::complex<double> > spheric_harmonic_cache;
    extern bool shperic_harmonic_cache_on;
    
    bool GetSphericalHarmonicCache();
    
    void SetSphericalHarmonicCache(bool turn_on);
    
    size_t GetSphericalHarmonicCacheSize();
    
    void ClearSphericalHarmonicCache();
    
    size_t GetSphericalHarmonicCacheStats();
    
    /**
     * @brief Compute spherical harmonic Y_l^m(theta,phi)
     *
     * Uses the boost implementation of spherical harmonic to compute the amplitude
     * @param l l
     * @param m m
     * @param theta theta
     * @param phi phi
     * @return Complex spherical harmonic
     */
    std::complex<double> SphericalHarmonic(const unsigned int l, const int m, const double theta, const double phi);
    
    /**
     * @brief Computes relativistic spin Breit-Wigner with mass dependant width
     *
     * @param m mass
     * @param mass_J nominal mass
     * @param width_J nominal width
     * @param r interaction radius (Blatt-Weisskopf form factor)
     * @param J Spin
     * @param q daughter momentum in the resonance system at mass m
     * @param q_J daughter momentum in the resonance system at mass mass_J
     * @return Complex relativistic spin Breit-Wigner amplitude
     */
    std::complex<double> RelativisticSpinBreitWigner(const double m, const double mass_J, const double width_J,
                                                     const double r, const unsigned int J, const double q, const double q_J);
    
    /**
     * @brief Computes Flatte formalism amplitude
     *
     * @param m invariant mass m_ab of the resonance
     * @param mass_R nominal mass of the resonance
     * @param g vector with coupling constants
     * @param c_mass vector with pairs of invariant daughter masses m_a and m_b
     * @param q daughter momentum in the resonance system at mass m
     * @return Complex amplitude according to Flatte formalism
     */
    std::complex<double> FlatteFormalism(const double m, const double mass_R,
                                         const std::vector<double> &g, const std::vector<std::pair<double,double> > &c_mass, const double q);
    
    typedef std::tuple<unsigned int, unsigned int, int, double, double, double> tuple_helicity_amplitude;
    extern std::unordered_map<tuple_helicity_amplitude, std::complex<double> > helicity_amplitude_cache;
    extern bool helicity_amplitude_cache_on;
    
    bool GetHelicityAnglesCache();
    
    void SetHelicityAnglesCache(bool turn_on);
    
    size_t GetHelicityAnglesCacheSize();
    
    void ClearHelicityAnglesCache();
    
    size_t GetHelicityAnglesCacheStats();
    
    /**
     * @brief Computes complex angular distribution
     *
     * Computes complex angular distribution in the decay of a spinless particle to two particles X1,X2 with spins J1,J2.
     * Each of the two particles decays strongly to two pseudoscalars, thus conserving parity.
     * @param phi angle phi in helicity frame
     * @param th1 angle theta_1 in helicity frame
     * @param th2 angle theta_2 in helicity frame
     * @param J1 spin of particle 1 (the particle theta_1 is related)
     * @param J2 spin of particle 2 (the particle theta_2 is related)
     * @param A vector of complex helicity amplitudes, size must be 2*min(J1,J2)+1 and the internal order must be -min(J1,J2),..,0,..,min(J1,J2)
     * @return Complex angular distribution
     */
    std::complex<double> HelicityAngles(const double phi, const double th1, const double th2,
                                        const unsigned int J1, const unsigned int J2, const std::vector<std::complex<double> > &A);
    
    /**
     * @brief Computes LASS PDF for (KPi)_0^0 scattering below 1.6 GeV/c
     *
     * See also Nuclear Physics B296 (1988) 493-526
     * @param m mass
     * @param mass_0 nominal mass
     * @param width_0 nominal width
     * @param a a
     * @param b b
     * @param q daughter momentum in the resonance system at mass m
     * @param q_0 daughter momentum in the resonance system at mass mass_0
     * @return Complex amplitude of the LASS parameterization for KPi scattering
     */
    std::complex<double> LASS(const double m, const double mass_0, const double width_0,
                              const double a, const double b, const double q, const double q_0);
    /**
     * @brief Computes LASS PDF for (KPi)_0^0 scattering below a treshold
     *
     * See also Nucl.Phys.B296, 493 (1988)
     * @param m mass
     * @param m0 nominal mass
     * @param g0 nominal width
     * @param a scattering length
     * @param r effective range
     * @param cutoff cutoff
     * @param q daughter momentum in the resonance system at mass m
     * @param q_0 daughter momentum in the resonance system at mass mass_0
     * @return Complex amplitude of the LASS parameterization for KPi scattering
     */
    std::complex<double> LASS(const double m, const double m0, const double g0,
                              const double a, const double r, const double cutoff, const double q, const double q0);
    
    /**
     * @brief Computes Variance Gamma Distribution for given value x
     * @param x observable
     * @param mu location
     * @param alpha tail
     * @param beta asymmetry
     * @param lambda scale
     * @return P(x)
     */
    double VarianceGammaDistribution(const double x, const double mu, const double alpha, const double beta, const double lambda);
    
    /**
     * @brief Computes mean of Variance Gamma Distribution with given parameters
     * @param mu location
     * @param alpha tail
     * @param beta asymmetry
     * @param lambda scale
     * @return mean
     */
    double VarianceGammaDistribution_GetMean(const double mu, const double alpha, const double beta, const double lambda);
    
    /**
     * @brief Computes variance of Variance Gamma Distribution with given parameters
     * @param mu location
     * @param alpha tail
     * @param beta asymmetry
     * @param lambda scale
     * @return variance
     */
    double VarianceGammaDistribution_GetVariance(const double mu, const double alpha, const double beta, const double lambda);
    
    /**
     * @brief Computes students t distribution for given value x
     *
     * See also http://en.wikipedia.org/wiki/Student%27s_t-distribution  (Three parameter version)
     * @param x observable
     * @param mu location
     * @param lambda scale parameter (1/sqrt(lambda) corresponds to width)
     * @param nu dgf
     * @return P(x)
     */
    double StudentsTDistribution(const double x, const double mu, const double lambda, const double nu);
    
    /**
     * @brief Computes mean of students t distribution for given value x
     *
     * See also http://en.wikipedia.org/wiki/Student%27s_t-distribution  (Three parameter version)
     * @param mu location
     * @param lambda scale parameter (1/sqrt(lambda) corresponds to width)
     * @param nu dgf
     * @return P(x)
     */
    double StudentsTDistribution_GetMean(const double mu, const double lambda, const double nu);
    
    /**
     * @brief Computes variance of students t distribution for given value x
     *
     * See also http://en.wikipedia.org/wiki/Student%27s_t-distribution  (Three parameter version)
     * @param mu location
     * @param lambda scale parameter (1/sqrt(lambda) corresponds to width)
     * @param nu dgf
     * @return P(x)
     */
    double StudentsTDistribution_GetVariance(const double mu, const double lambda, const double nu);
    
    
    /**
     * @brief Computes complete matrix element for a relativistic spin Breit-Wigner resonance
     *
     * @param mass_12 first Dalitz variable B -> (12) 3
     * @param mass_23 second Dalitz variable B -> 1 (23)
     * @param resonance combination in which the resonance if
     * @param angular combination in which the angular distribution is expressed
     * @param mass_J nominal mass of intermeediate resonance
     * @param width_J nominal width of intermeediate resonance
     * @param r interaction radius (Blatt-Weisskopf form factor) of intermeediate resonance
     * @param J Spin of intermeediate resonance
     * @param M reosnance mother mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @return Complex relativistic spin Breit-Wigner amplitude
     */
    std::complex<double> GetBreitWignerMatrixElement(const double mass_12, const double mass_23, COMBINATION resonance, COMBINATION angular, const double mass_J, const double width_J, const double r, const unsigned int J, const double M, const double m1, const double m2, const double m3);
    
    
    
    /**
     * @brief Computes complete matrix element for a LASS distribution
     *
     * @param mass_12 first Dalitz variable B -> (12) 3
     * @param mass_23 second Dalitz variable B -> 1 (23)
     * @param resonance combination in which the resonance if
     * @param m0 nominal mass
     * @param g0 nominal width
     * @param a scattering length
     * @param r effective range
     * @param cutoff cutoff
     * @param M reosnance mother mass
     * @param m1 inv. mass 1st daughter
     * @param m2 inv. mass 2nd daughter
     * @param m3 inv. mass 3rd daughter
     * @return Complex relativistic spin Breit-Wigner amplitude
     */
    std::complex<double> GetLASSMatrixElement(const double mass_12, const double mass_23, COMBINATION resonance, const double m0, const double g0, const double a, const double r, const double cutoff, const double M, const double m1, const double m2, const double m3);
    
    
    
} // ComplexPDF

#endif /* LIBCOMPLEXPDF_H_ */
