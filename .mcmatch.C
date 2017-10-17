
    /** Flags that describe different reconstruction errors. */
    enum MCErrorFlags {
      c_Correct             = 0,  /**< This Particle and all its daughters are perfectly reconstructed. */
      c_MissFSR             = 1, /**< A Final State Radiation (FSR) photon is not reconstructed (based on MCParticle::c_IsFSRPhoton). */
      c_MissingResonance    = 2, /**< The associated MCParticle decay contained additional non-final-state particles (e.g. a rho) that weren't reconstructed. This is probably O.K. in most cases*/
      c_DecayInFlight       = 4, /**< A Particle was reconstructed from the secondary decay product of the actual particle. This means that a wrong hypothesis was used to reconstruct it, which e.g. for tracks might mean a pion hypothesis was used for a secondary electron. */
      c_MissNeutrino        = 8, /**< A neutrino is missing (not reconstructed). */
      c_MissGamma           = 16, /**< A photon (not FSR) is missing (not reconstructed). */
      c_MissMassiveParticle = 32, /**< A generated massive FSP is missing (not reconstructed). */
      c_MissKlong           = 64, /**< A Klong is missing (not reconstructed). */
      c_MisID               = 128, /**< One of the charged final state particles is mis-identified. */
      c_AddedWrongParticle  = 256, /**< A non-FSP Particle has wrong PDG code, meaning one of the daughters (or their daughters) belongs to another Particle. */
      c_InternalError       = 512, /**< There was an error in MC matching. Not a valid match. Might indicate fake/background track or cluster. */
      c_MissPHOTOS          = 1024, /**< A photon created by PHOTOS was not reconstructed (based on MCParticle::c_IsPHOTOSPhoton) */
    };



