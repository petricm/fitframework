/**
 * @file	libComplexPDF.cc
 * @date	Nov 9, 2013
 * @author	mprim, mpetric
 * @brief	brief description
 *
 * long description
 */

#include "libComplexPDF.h"
#include <iostream>
#include <utility>
#include <cmath>
#include "boost/math/constants/constants.hpp"
#include "boost/math/special_functions/gamma.hpp"
#include "boost/math/special_functions/bessel.hpp"
#include "boost/math/special_functions/spherical_harmonic.hpp"

namespace std {
	namespace {
        
        // Code from boost
        // Reciprocal of the golden ratio helps spread entropy
        //     and handles duplicates.
        // See Mike Seymour in magic-numbers-in-boosthash-combine:
        //     http://stackoverflow.com/questions/4948780
        template <class T>
        inline void hash_combine(std::size_t& seed, T const& v)
        {
            seed ^= hash<T>()(v) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        
        // Recursive template code derived from Matthieu M.
        template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
        struct HashValueImpl
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                HashValueImpl<Tuple, Index-1>::apply(seed, tuple);
                hash_combine(seed, get<Index>(tuple));
            }
        };
        
        template <class Tuple>
        struct HashValueImpl<Tuple,0>
        {
            static void apply(size_t& seed, Tuple const& tuple)
            {
                hash_combine(seed, get<0>(tuple));
            }
        };
        
	}
    
    template <typename ... TT>
    struct hash<std::tuple<TT...>>
    {
        size_t
        operator()(std::tuple<TT...> const& tt) const
        {
            size_t seed = 0;
            HashValueImpl<std::tuple<TT...> >::apply(seed, tt);
            return seed;
        }
    };
}

namespace ComplexPDF {
    
    double CalculateMomentum(double m, const double m1, const double m2) {
        double add_12 = m1 + m2;
        double sub_12 = m1 - m2;
        
        return std::sqrt((m*m - add_12*add_12)*(m*m - sub_12*sub_12))/(2.0*m);
    }
    
    bool InsideDalitzLimits(const double m12, const double m23, const double M, const double m1, const double m2, const double m3) {
        // check left-right boundaries
        if(m12*m12 < (m1+m2)*(m1+m2) || m12*m12 > (M-m3)*(M-m3))
            return false;
        // check upper-lower boundaries
        double m23_min_2;
        double m23_max_2;
        ComplexPDF::DalitzLimits(m12,M,m1,m2,m3,m23_max_2,m23_min_2);
        if(m23*m23 < m23_min_2 || m23*m23 > m23_max_2)
            return false;
        // we are inside the physically allowed region
        return true;
    }
    
    double CLEOThresholdFunction(const double v, const double Ec, const double Eth, const double v_max) {
        static const double pi = boost::math::constants::pi<double>();
        
        if ((0 < Eth * std::abs(v-v_max)) && (Eth * std::abs(v-v_max) < pi/2.0 )) {
            return Ec + ( 1.0 - Ec ) * std::sin( Eth * std::abs(v-v_max) ); //sine function at the edge od the dalitz plot.
        }else{
            return 1.0;
        }
        
        return -1.0;
    
    
    }
    
    void DalitzLimits(const double m, const double M, const double m1, const double m2, const double m3, double &m23_max_2, double &m23_min_2) {
        double E_2 = (m*m - m1*m1 + m2*m2)/(2*m);
        double E_3 = (M*M - m*m - m3*m3)/(2*m);
        
        m23_max_2 = (E_2+E_3)*(E_2+E_3) - ( std::sqrt(E_2*E_2 - m2*m2) - std::sqrt(E_3*E_3 - m3*m3) ) * ( std::sqrt(E_2*E_2 - m2*m2) - std::sqrt(E_3*E_3 - m3*m3) );
        m23_min_2 = (E_2+E_3)*(E_2+E_3) - ( std::sqrt(E_2*E_2 - m2*m2) + std::sqrt(E_3*E_3 - m3*m3) ) * ( std::sqrt(E_2*E_2 - m2*m2) + std::sqrt(E_3*E_3 - m3*m3) );
    }
    
    double DalitzAngularDistribution(const double m12, const double m23, const unsigned int J, const double m,
                                     const double M, const double m1, const double m2, const double m3, ANGULAR_FORMALISM formalism) {
        double MAB2 = m12*m12;
        double MBC2 = m23*m23;
        
        double mr2;
        switch(formalism) {
            case ComplexPDF::HELICITY: // Helicty formalism uses the nominal resonance mass
                mr2 = m*m;
                break;
            case ComplexPDF::ZEMACH: // Zemach formalism uses the running inv. mass of daugther particles
                mr2 = MAB2;
                break;
            default:
                std::cerr << "ERROR: Angular formalism " << formalism << " not implemented in DalitzAngularDistribution(..) in libComplexPDF" << std::endl;
                std::exit(EXIT_FAILURE);
        }
        
        double M2 = M*M;
        double mA2 = m1*m1;
        double mB2 = m2*m2;
        double mC2 = m3*m3;
        double MAC2 = M2 + mA2 + mB2 + mC2 - MAB2 - MBC2;
        
        if(J == 0) {
            // calc angular distribution for Spin J == 0
            return 1.0;
        } else if (J == 1) {
            // calc angular distribution for Spin J == 1
            return MAC2 - MBC2 + (M2 - mC2)*(mB2 - mA2)/mr2;
        } else if(J == 2) {
            // calc angular distribution for Spin J == 2
            double result = MAC2 - MBC2 + (M2 - mC2)*(mB2 - mA2)/mr2;
            result *= result;
            return result - (1.0/3.0) * (MAB2 - 2*M2 - 2*mC2 + (M2 - mC2)*(M2 - mC2)/mr2) * (MAB2 - 2*mA2 - 2*mB2 + (mA2 - mB2)*(mA2 - mB2)/mr2);
        } else {
            std::cerr << "ERROR: Spin " << J << " not implemented for DalitzAngularDistribution(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    
    std::complex<double> DalitzPhaseSpaceAmplitude(const double /*m12*/, const double /*m23*/) {
        return std::polar(1.0,0.0);
    }
    
    double DalitzCubicPolynomial(const double m12, const double m23,
                                 const double E_x, const double E_y,
                                 const double E_xx, const double E_xy, const double E_yy,
                                 const double E_xxx, const double E_xxy, const double E_xyy, const double E_yyy,
                                 const double m12_offset, const double m23_offset) {
        double x = m12-m12_offset;
        double y = m23-m23_offset;
        double x2 = x*x;
        double y2 = y*y;
        double x3 = x2*x;
        double y3 = y2*y;
        
        return std::abs(1 + E_x*x + E_y*y + E_xx*x2 + E_xy*x*y * E_yy*y2 + E_xxx*x3 + E_xxy*x2*y + E_xyy*x*y2 + E_yyy*y3);
    }
    
    double DalitzCubicPolynomialSymmetric(const double m12, const double m23,
                                          const double E_1, const double E_2, const double E_3,
                                          const double E_xy, const double E_xyn,
                                          const double m12_offset, const double m23_offset) {
        double x = m12-m12_offset;
        double y = m23-m23_offset;
        double x2 = x*x;
        double y2 = y*y;
        double x3 = x2*x;
        double y3 = y2*y;
        
        return 1 + E_1*(x+y) + E_2*(x2+y2) + E_3*(x3*y3) + E_xy*x*y + E_xyn*(x2*y + x*y2);
    }
    
    double BlattWeisskopfFormFactor(const double q, const double q_J, const double r, const unsigned int J) {
        if(J == 0) {
            // calc Blatt-Weisskopf form factor for Spin J == 0
            return 1.0;
        }
        
        double r2 = r*r;
        double q2 = q*q;
        double q_J2 = q_J*q_J;
        
        if (J == 1) {
            // calc Blatt-Weisskopf form factor for Spin J == 1
            return std::sqrt((1 + r2*q_J2) / (1 + r2*q2));
        } else if(J == 2) {
            // calc Blatt-Weisskopf form factor for Spin J == 2
            return std::sqrt((9 + 3*r2*q_J2 + r2*r2*q_J2*q_J2) / (9 + 3*r2*q2 + r2*r2*q2*q2));
        } else {
            std::cerr << "ERROR: Spin " << J << " not implemented for BlattWeisskopfFormFactor(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    
    double PhaseSpaceFactor(const double m, const double M, const double m1, const double m2, const double m3) {
        double m23_max_2 = 0;
        double m23_min_2 = 0;
        DalitzLimits(m,M,m1,m2,m3,m23_max_2,m23_min_2);
        
        return 2 * m * (m23_max_2 - m23_min_2);
    }
    
    double ThreeBodyPhaseSpaceProjection(const double m12, const double M, const double m1, const double m2, const double m3) {
        double a = std::sqrt(-m2*m2 + (-m1*m1 + m12*m12 + m2*m2)*(-m1*m1 + m12*m12 + m2*m2) / (4*m12*m12) )
        - std::sqrt(-m3*m3 + (M*M - m12*m12 - m3*m3)*(M*M - m12*m12 - m3*m3) / (4*m12*m12) );
        double b = std::sqrt(-m2*m2 + (-m1*m1 + m12*m12 + m2*m2)*(-m1*m1 + m12*m12 + m2*m2) / (4*m12*m12) )
        + std::sqrt(-m3*m3 + (M*M - m12*m12 - m3*m3)*(M*M - m12*m12 - m3*m3) / (4*m12*m12) );
        return 2*m12*(b*b - a*a);
    }
    
    double TwoBodyThresholdFunction(const double m, const double a, const double m_min) {
        if(m < m_min)
            return 0.0;
        if(a < 0 || m_min < 0)
            return 0.0;
        
        return std::atan(std::sqrt((m-m_min)/a));
    }
    
    std::unordered_map<tuple_spheric_harmonic, std::complex<double> > spheric_harmonic_cache;
    bool shperic_harmonic_cache_on = false;
    size_t spheric_harmonic_cache_stats = 0;
    
    bool GetSphericalHarmonicCache() {
        return shperic_harmonic_cache_on;
    }
    
    void SetSphericalHarmonicCache(bool turn_on) {
        shperic_harmonic_cache_on = turn_on;
    }
    
    size_t GetSphericalHarmonicCacheSize() {
        return spheric_harmonic_cache.size();
    }
    
    void ClearSphericalHarmonicCache() {
        spheric_harmonic_cache_stats = 0;
        return spheric_harmonic_cache.clear();
    }
    
    size_t GetSphericalHarmonicCacheStats() {
        return spheric_harmonic_cache_stats;
    }
    
    std::complex<double> SphericalHarmonic(const unsigned int l, const int m, const double theta, const double phi) {
        if(shperic_harmonic_cache_on) {
            const tuple_spheric_harmonic key = std::make_tuple(l,m,theta,phi);
            std::unordered_map<tuple_spheric_harmonic, std::complex<double> >::iterator it;
            it = spheric_harmonic_cache.find(key);
            if(it != spheric_harmonic_cache.end()) {
                ++spheric_harmonic_cache_stats;
                return (*it).second;
            } else {
                std::complex<double> result = boost::math::spherical_harmonic(l,m,theta,phi);
                spheric_harmonic_cache[key] = result;
                return result;
            }
        }
        return boost::math::spherical_harmonic(l,m,theta,phi);
    }
    
    std::complex<double> RelativisticSpinBreitWigner(const double m, const double mass_J, const double width_J,
                                                     const double r, const unsigned int J, const double q, const double q_J) {
        // calculate some helpers
        double q_ratio = q / q_J;
        double m_ratio = mass_J / m;
        double mass_J2 = mass_J*mass_J;
        double m2 = m*m;
        
        // phase delta_J
        std::complex<double> amplitude;
        if(J == 0) {
            // calc mass dependant width for Spin J == 0
            double width = width_J * ( m_ratio ) * ( q_ratio );
            // calculate amplitude
            std::complex<double> amp(mass_J2 - m2, -1.0* mass_J * width);
            amplitude=1.0/amp;
        } else if (J == 1) {
            // calc mass dependant width for Spin J == 1
            double width = width_J;// * ( m_ratio ) * ( BlattWeisskopfFormFactor(q,q_J,r,1) * BlattWeisskopfFormFactor(q,q_J,r,1) ) * std::pow( q_ratio, 3 );
            // calculate amplitude
            std::complex<double> amp(mass_J2 - m2, -1.0* mass_J * width);
            amplitude=1.0/amp;
        } else if(J == 2) {
            // calc mass dependant width for Spin J == 2
            double width = width_J * ( m_ratio ) * ( BlattWeisskopfFormFactor(q,q_J,r,2) * BlattWeisskopfFormFactor(q,q_J,r,2) ) * std::pow( q_ratio, 5 );
            // calculate amplitude
            std::complex<double> amp(mass_J2 - m2, -1.0* mass_J * width);
            amplitude=1.0/amp;
        } else {
            std::cerr << "ERROR: Spin " << J << " not implemented for RelativisticSpinBreitWigner(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        return amplitude;
    }
    
    std::complex<double> FlatteFormalism(const double m, const double mass_R,
                                         const std::vector<double> &g, const std::vector<std::pair<double,double> > &c_mass, const double q) {
        static const std::complex<double> I(0.0,1.0);
        if(g.size() != c_mass.size()) {
            std::cerr << "ERROR: Size of couplings g=" << g.size() << " not equal to size of channel children masses c_mass=" << c_mass.size() << " in FlatteFormalism(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        std::complex<double> sum(0.0,0.0);
        std::size_t g_size = g.size();
        for(std::size_t i = 0; i < g_size; ++i) {
            double arg = (1 - ((c_mass[i].first - c_mass[i].second)/m) * ((c_mass[i].first - c_mass[i].second)/m) )
            *(1 - ((c_mass[i].first + c_mass[i].second)/m) * ((c_mass[i].first + c_mass[i].second)/m) );
            if(arg >= 0) {
                std::complex<double> rho(std::sqrt(arg),0.0);
                sum += rho*g[i]*g[i];
            } else {
                std::complex<double> rho(0.0,std::sqrt(-arg));
                sum += rho*g[i]*g[i];
            }
        }
        
        // compute amplitude
        std::complex<double> Flatte = 1.0 / (mass_R*mass_R - m*m - I*sum);
        // return phase space factor in the explicitly reconstructed channel * amplitude
        return q * Flatte;
    }
    
    std::unordered_map<tuple_helicity_amplitude, std::complex<double> > helicity_amplitude_cache;
    bool helicity_amplitude_cache_on = false;
    size_t helicity_amplitude_cache_stats = 0;
    
    bool GetHelicityAnglesCache() {
        return helicity_amplitude_cache_on;
    }
    
    void SetHelicityAnglesCache(bool turn_on) {
        helicity_amplitude_cache_on = turn_on;
    }
    
    size_t GetHelicityAnglesCacheSize() {
        return helicity_amplitude_cache.size();
    }
    
    void ClearHelicityAnglesCache() {
        helicity_amplitude_cache_stats = 0;
        return helicity_amplitude_cache.clear();
    }
    
    size_t GetHelicityAnglesCacheStats() {
        return helicity_amplitude_cache_stats;
    }
    
    std::complex<double> HelicityAngles(const double phi, const double th1, const double th2,
                                        const unsigned int J1, const unsigned int J2, const std::vector<std::complex<double> > &A) {
        static const double pi = boost::math::constants::pi<double>();
        // check if amplitue vector has proper size
        if(A.size() != 2*std::min(J1, J2) + 1) {
            std::cerr << "ERROR: Number of amplitudes " << A.size() << " doesn't equal number calculated from 2*min(J1,J2)+1="
            << 2*std::min(J1, J2) + 1 << " (J1=" << J1 << ", J2=" << J2 << ") in HelicityAngles(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        // running variable for lambda
        int lambda = -1 * std::min(J1,J2);
        // result amplitude
        std::complex<double> amp(0,0);
        // loop over all amplitudes, thus all values of lambda
        std::size_t A_size = A.size();
        if(helicity_amplitude_cache_on) {
            std::complex<double> result;
            std::unordered_map<tuple_helicity_amplitude, std::complex<double> >::iterator it;
            for(std::size_t i = 0; i < A_size; ++i) {
                // lookup product of two spherical harmonics
                //const tuple_helicity_amplitude key = std::make_tuple(J1,J2,lambda,th1,th2,phi);
                it = helicity_amplitude_cache.find(std::make_tuple(J1,J2,lambda,th1,th2,phi));
                if(it != helicity_amplitude_cache.end()) {
                    ++helicity_amplitude_cache_stats;
                    result = (*it).second;
                } else {
                    result = SphericalHarmonic(J1,lambda,th1,(pi-phi)) * SphericalHarmonic(J2,-lambda,-th2,0);
                    helicity_amplitude_cache[std::make_tuple(J1,J2,lambda,th1,th2,phi)] = result;
                }
                // multiply result
                amp += A[i] * result;
                lambda += 1;
            }
        } else {
            for(std::size_t i = 0; i < A_size; ++i) {
                amp += A[i] * SphericalHarmonic(J1,lambda,th1,(pi-phi)) * SphericalHarmonic(J2,-lambda,-th2,0);
                lambda += 1;
            }
        }
        // return complex angular distribution
        return amp;
    }
    
    std::complex<double> LASS(const double m, const double mass_0, const double width_0,
                              const double a, const double b, const double q, const double q_0) {
        
        // get resonant J == 0 component of K_0*(1430)^0
        std::complex<double> BW = RelativisticSpinBreitWigner(m,mass_0,width_0,0,0,q,q_0);
        // get resonant phase
        double delta_R = arg(BW);
        // get non-resonant phase
        double delta_B = std::atan((1.0 / (1.0 / (a*q) + 0.5*b*q )));
        
        // calculate r and phi in polar coordinates
        std::complex<double> amplitude = std::polar(std::sin(delta_R+delta_B), delta_R+delta_B);
        // return complex amplitude
        return amplitude;
    }
    
    std::complex<double> LASS(const double m, const double m0, const double g0,
                              const double a, const double r, const double cutoff, const double q, const double q0) {
        
        /*
         
         Parameterization of Kpi S-wave using LASS scattering data.
         - Nucl.Phys.B296, 493 (1988)
         - W.Dunwoodie,http://www.slac.stanford.edu/~wmd/kpi_swave/kpi_swave_fit.note
         
         m                                     m0^2*Gamma0/q0
         ----------------- + exp(2*i*delta) * --------------------------------
         q*cot(delta)-i*q                     m0^2-m^2 - i*m0*Gamma0*q/m*m0/q0
         
         
         where q = momentum of K or pi in Kpi system
         
         q*cot(delta) = 1/ a   + 1/2 * [ r * q**2 ]
         
         a = scattering length
         
         r = effective range
         
         */
        
        
        //! This is a direct transcription from EvtLASSAmp.cpp
        
        
        // elastic scattering
        double qcotd = 1./a + 0.5*r*q*q;
        
        std::complex<double> lass_elastic(0,q);
        if (m<cutoff) {
            lass_elastic = m / ( qcotd - lass_elastic );
        }else{
            lass_elastic = 0;
        }
        
        // relative phase
        double cosd = 1;
        double sind = 0;
        if ( q > 0 ) {
            cosd = qcotd * qcotd / ( q * q);
            cosd = std::sqrt( cosd / ( 1 + cosd ) );
            sind = std::sqrt( 1 - cosd * cosd );
        }
        
        std::complex<double> lass_phase( cosd , sind );
        lass_phase *= lass_phase;
        
        // K*(1430)
        double gamma = g0 * q/m * m0/q0;
        std::complex<double> lass_Kstar(0.,m0*gamma);
        lass_Kstar = (m0*m0)*(g0/q0)/(m0*m0-m*m-lass_Kstar);


        //the amplitude
        std::complex<double> amplitude = lass_phase * lass_Kstar + lass_elastic;

        
        return amplitude;
        
    }
    
    
    double VarianceGammaDistribution(const double x, const double mu, const double alpha, const double beta, const double lambda) {
        static const double sqrt_pi = boost::math::constants::root_pi<double>();
        // check additional conditions
        if(lambda <= 0) {
            std::cerr << "ERROR: Parameter lambda needs to be larger than 0 in VarianceGammaDistribution(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if(alpha <= std::max(0.0,fabs(beta))) {
            std::cerr << "ERROR: Parameter alpha needs to be larger than max(0,abs(beta)) in VarianceGammaDistribution(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        double arg = x - mu;
        // avoid numeric divergence of Bessel function
        if(fabs(arg) < 1e-12) {
            arg = 1e-12;
            std::cerr << "WARNING: [x - mu < 1e-12] in VarianceGammaDistribution(..) in libComplexPDF. Evaluate at 1e-12 to avoid divergency of modified Bessel function of the 2nd kind." << std::endl;
        }
        double gamma = std::sqrt(alpha*alpha - beta*beta);
        double K = boost::math::cyl_bessel_k(lambda - 0.5, alpha * fabs(arg));
        double G = boost::math::tgamma(lambda);
        double value = std::pow(gamma,2*lambda) * std::pow(fabs(arg), lambda - 0.5) * K * exp(beta * (arg));
        value /= sqrt_pi * G * std::pow((2*alpha),lambda - 0.5);
        return value;
    }
    
    double VarianceGammaDistribution_GetMean(const double mu, const double alpha, const double beta, const double lambda) {
        // check additional conditions
        if(lambda <= 0) {
            std::cerr << "ERROR: Parameter lambda needs to be larger than 0 in VarianceGammaDistribution_GetMean(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if(alpha <= std::max(0.0,fabs(beta))) {
            std::cerr << "ERROR: Parameter alpha needs to be larger than max(0,abs(beta)) in VarianceGammaDistribution_GetMean(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        return mu + (2 * beta * lambda) / (alpha*alpha - beta*beta);
    }
    
    double VarianceGammaDistribution_GetVariance(const double /*mu*/, const double alpha, const double beta, const double lambda) {
        // check additional conditions
        if(lambda <= 0) {
            std::cerr << "ERROR: Parameter lambda needs to be larger than 0 in VarianceGammaDistribution_GetMean(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        if(alpha <= std::max(0.0,fabs(beta))) {
            std::cerr << "ERROR: Parameter alpha needs to be larger than max(0,abs(beta)) in VarianceGammaDistribution_GetMean(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        return ((2 * lambda) / (alpha*alpha - beta*beta)) * (1 + (2*beta*beta) / (alpha*alpha - beta*beta));
    }
    
    double StudentsTDistribution(const double x, const double mu, const double lambda, const double nu) {
        static const double pi = boost::math::constants::pi<double>();
        double g = boost::math::tgamma((nu+1.0)/2.0) / boost::math::tgamma(nu/2.0);
        double s = std::sqrt(lambda/pi/nu);
        double f = std::pow(1.0 + lambda*(x-mu)*(x-mu)/nu, -(nu+1)/2.0);
        return g*s*f;
    }
    
    double StudentsTDistribution_GetMean(const double mu, const double /*lambda*/, const double nu) {
        if(nu > 1.0)
            return mu;
        else {
            std::cerr << "ERROR: Mean of students T not defined for nu <= 1 in StudentsT_GetMean(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    
    double StudentsTDistribution_GetVariance(const double /*mu*/, const double lambda, const double nu) {
        if(nu > 2.0)
            return (1.0/lambda) * (nu/(nu - 2));
        else {
            std::cerr << "ERROR: Variance of students T not defined for nu <= 2 in StudentsT_GetVariance(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
    }
    
    std::complex<double> GetBreitWignerMatrixElement(const double mass_12, const double mass_23, COMBINATION resonance, COMBINATION angular, const double mass_J, const double width_J,const double r, const unsigned int J, const double M, const double m1, const double m2, const double m3){
        
        const double mass_31 = std::sqrt(M*M + m1*m1 + m2*m2 + m3*m3 - mass_12*mass_12 - mass_23*mass_23);
        
        if( std::isnan(mass_31) || std::isinf(mass_31)) {
            std::cerr << "ERROR: the calculated third Dalitz variable is NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        double M_Res = 0;
        double M_Ang = 0;
        double M_A = 0;
        double M_B = 0;
        double M_C = 0;
        
        switch(resonance) {
            case ComplexPDF::AB:
                M_Res = mass_12;
                M_A = m1;
                M_B = m2;
                M_C = m3;
                break;
            case ComplexPDF::BC:
                M_Res = mass_23;
                M_A = m2;
                M_B = m3;
                M_C = m1;
                break;
            case ComplexPDF::CA:
                M_Res = mass_31;
                M_A = m3;
                M_B = m1;
                M_C = m2;
                break;
            default:
                std::cerr << "ERROR: resonance combination " << resonance << " not possible in GetBreitWignerMatrixElement(..) in libComplexPDF" << std::endl;
                std::exit(EXIT_FAILURE);
        }
        
        switch(angular) {
            case ComplexPDF::AB:
                M_Ang = mass_12;
                break;
            case ComplexPDF::BC:
                M_Ang = mass_23;
                break;
            case ComplexPDF::CA:
                M_Ang = mass_31;
                break;
            default:
                std::cerr << "ERROR: angular combination " << angular << " not possible in GetBreitWignerMatrixElement(..) in libComplexPDF" << std::endl;
                std::exit(EXIT_FAILURE);
        }

        const double pAB = ComplexPDF::CalculateMomentum(M_Res, M_A, M_B);
        const double PAB = ComplexPDF::CalculateMomentum(M, M_Res, M_C);
        
        if( std::isnan(pAB) || std::isnan(PAB) || std::isinf(pAB) || std::isinf(PAB)) {
            std::cerr << "ERROR: transformation to resonance frame NAN of INF in GetBreitWignerMatrixElement(..) in libComplexPDF" << std::endl;
            std::cerr <<"pAB: "<<pAB<<std::endl;
            std::cerr <<"PAB: "<<PAB<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        const double p_Res = ComplexPDF::CalculateMomentum(mass_J, M_A, M_B );
        const double P_Res = ComplexPDF::CalculateMomentum(M, mass_J, M_C);

        if( std::isnan(p_Res) || std::isnan(P_Res) || std::isinf(p_Res) || std::isinf(P_Res)) {
            std::cerr << "ERROR: transformation to resonance frame NAN or INF in GetBreitWignerMatrixElement(..) in libComplexPDF" << std::endl;
            std::cerr <<"p_Res: "<<p_Res<<std::endl;
            std::cerr <<"P_Res: "<<P_Res<<std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        //calculate Blatt Weisskopf Form Factors
        const double BW_R = ComplexPDF::BlattWeisskopfFormFactor(pAB, p_Res, r, J); //for resonannce
        const double BW_B = ComplexPDF::BlattWeisskopfFormFactor(PAB, P_Res, 0, 0); //for B, D meson -> but it is actually 1, since spin 0 ->this line only here for future reference
        
        //calculate Dalitz angular distribution
        const double DAD = ComplexPDF::DalitzAngularDistribution(M_Res, M_Ang, J, mass_J, M, M_A, M_B, M_C, ComplexPDF::ZEMACH);
        
        //calculate Relativistic Breit Wigner distribution
        std::complex<double> RBW = ComplexPDF::RelativisticSpinBreitWigner(M_Res, mass_J, width_J, r, J, pAB, p_Res);
        
        if( std::isnan(BW_R)) {
            std::cerr << "ERROR: BW_R is NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        if( std::isnan(BW_B)) {
            std::cerr << "ERROR: BW_B is NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        if( std::isnan(DAD)) {
            std::cerr << "ERROR: DAD is NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        if( RBW!=RBW) {
            std::cerr << "ERROR: RBW is NAN!" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        return BW_B * BW_R * DAD * RBW;
        
        
    }
    
    std::complex<double> GetLASSMatrixElement(const double mass_12, const double mass_23, COMBINATION resonance, const double m0, const double g0, const double a, const double r, const double cutoff, const double M, const double m1, const double m2, const double m3){
        
        const double mass_31 = std::sqrt(M*M + m1*m1 + m2*m2 + m3*m3 - mass_12*mass_12 - mass_23*mass_23);
        
        double M_Res = 0;
        double M_A = 0;
        double M_B = 0;
        
        switch(resonance) {
            case ComplexPDF::AB:
                M_Res = mass_12;
                M_A = m1;
                M_B = m2;
                break;
            case ComplexPDF::BC:
                M_Res = mass_23;
                M_A = m2;
                M_B = m3;
                break;
            case ComplexPDF::CA:
                M_Res = mass_31;
                M_A = m3;
                M_B = m1;
                break;
            default:
                std::cerr << "ERROR: resonance combination " << resonance << " not possible in GetBreitWignerMatrixElement(..) in libComplexPDF" << std::endl;
                std::exit(EXIT_FAILURE);
        }
        
        const double pAB = ComplexPDF::CalculateMomentum(M_Res, M_A, M_B);
        
        if( std::isnan(pAB)) {
            std::cerr << "ERROR: transformation to resonance frame NAN in GetLASSMatrixElement(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        
        const double p_Res = ComplexPDF::CalculateMomentum(m0, M_A, M_B );
        
        if( std::isnan(p_Res)) {
            std::cerr << "ERROR: transformation to resonance frame NAN in GetLASSMatrixElement(..) in libComplexPDF" << std::endl;
            std::exit(EXIT_FAILURE);
        }
        
        //calculate Blatt Weisskopf Form Factors
        const double BW_R = 1; // LASS is valiod only for spin 0 resonances
        const double BW_B = 1; // LASS is valiod only for spin 0 resonances
        
        //calculate Dalitz angular distribution
        const double DAD = 1; //this is flat for a spin 0 resonance
        
        //calculate Relativistic Breit Wigner distribution
        std::complex<double> LASS = ComplexPDF::LASS(M_Res, m0, g0, a, r, cutoff, pAB, p_Res);
        
        return BW_B * BW_R * DAD * LASS;
        
        
    }
    
    
    
} // ComplexPDF
