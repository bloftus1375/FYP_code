#include "DetectorConstruction.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"
#include "G4LogicalVolume.hh"
#include "G4SystemOfUnits.hh"
#include "CLHEP/Units/SystemOfUnits.h"

{
	G4VPhysicalVolume* DetectorConstruction::Construct()
	{

        // Get nist material manager
        G4NistManager* nist = G4NistManager::Instance();


		// Construct the world:
        G4double worldSize = 2 * m;
        G4Material* vacuum = nist->FindOrBuildMaterial("G4_Galactic");

        auto solidWorld = new G4Box("World",
            worldSize / 2,
            worldSize / 2,
            worldSize);

        auto logicWorld = new G4LogicalVolume(solidWorld,
            vacuum, 
            "World");

        auto physWorld = new G4PVPlacement(nullptr,
            G4ThreeVector(), 
            logicWorld, 
            "World", 
            nullptr, 
            false, 
            0);

        // Create a 'linac head' object to contain all X-ray generation components
        G4double linacHeadThicknessXY = 15 * cm;
        G4double linacHeadThicknessZ = 15 * cm;

        G4ThreeVector linacHeadPos = G4ThreeVector(0 * cm, 0 * cm, -20 * cm);

        G4double headPhi = 0;
        G4double headTheta = 0;
        G4double headPsi = 0;
        G4RotationMatrix* linacHeadRotation = new G4RotationMatrix(headPhi, headTheta, headPsi);

        G4Box* solidHead = new G4Box("solidHead", linacHeadThicknessXY, linacHeadThicknessXY, linacHeadThicknessZ);
        G4LogicalVolume* logicHead = new G4LogicalVolume(solidHead, vacuum, "logicHead");
        G4VPhysicalVolume* physHead = new G4PVPlacement(
            linacHeadRotation,
            linacHeadPos,
            logicHead,
            "physHead",
            logicWorld,
            false,
            0);

        // set the member variable so we can get this volume in other parts of the program
        fLinacHead = physHead;

        // create a place for the particle gun to shoot from
        G4double particleGunAnchorThickness = 1 * mm;
        G4Box* solidParticleGunAnchor = new G4Box(
            "solidParticleGunAnchor", 
            particleGunAnchorThickness, 
            particleGunAnchorThickness, 
            particleGunAnchorThickness);

        G4ThreeVector particleGunAnchor1Pos = G4ThreeVector(0, 0, -linacHeadThicknessZ + 2 * cm);

        G4LogicalVolume* logicParticleGunAnchor1 = new G4LogicalVolume(solidParticleGunAnchor, vacuum, "logicParticleGunAnchor1");
        G4VPhysicalVolume* physAnchor1 = new G4PVPlacement(
            nullptr,
            particleGunAnchor1Pos,
            logicParticleGunAnchor1,
            "physParticleGunAnchor1",
            logicHead,
            false,
            0);


        // Set our member variables for the gun anchors:
        fParticleGunAnchor1 = physAnchor1;


        // create our tungsten target
        G4Material* tungsten = nist->FindOrBuildMaterial("G4_W");

        G4double innerTargetRadius = 0.0;
        G4double outerTargetRadius = 1.5 * cm;
        G4double targetThickness = 1 * mm;

        G4Tubs* solidTarget = new G4Tubs("Target",
            innerTargetRadius,
            outerTargetRadius,
            targetThickness / 2.0,
            0.0,
            360.0 * deg);

        G4LogicalVolume* logicTarget = new G4LogicalVolume(solidTarget, 
            tungsten, 
            "Target");

        // target position and rotation
        G4double targetZ = -linacHeadThicknessZ + 19.95 * cm;
        G4ThreeVector targetPos = G4ThreeVector(0, 0, targetZ); // 0,0,0
        G4RotationMatrix* targetRotation = new G4RotationMatrix();

        // place the target in the world
        new G4PVPlacement(
            targetRotation,
            targetPos,
            logicTarget,
            "Target",
            logicHead,
            false,
            0);

        // create tungsten collimator
        G4double innerColRadius = 4.0 * cm;
        G4double outerColRadius = 12. * cm;
        G4double colThickness = 10 * cm;

        G4Tubs* solidCol = new G4Tubs("Collimator",
            innerColRadius,
            outerColRadius,
            colThickness / 2.0,
            0.0,
            360.0 * deg);
        G4LogicalVolume* logicCol = new G4LogicalVolume(solidCol,
            tungsten,
            "Collimator");

        // collimator position and rotation
        G4double colZ =
            4.95*cm
            + (targetThickness / 2)
            + (colThickness / 2);

        G4ThreeVector colPos = G4ThreeVector(0, 0, colZ);

        G4RotationMatrix* colRotation = new G4RotationMatrix();

        // place the collimator in the world
        new G4PVPlacement(colRotation,
            colPos,
            logicCol,
            "Collimator",
            logicHead,
            false,
            0);
        

        // Detector
        G4double detectorSizeXY = 20 * cm;
        G4double detectorSizeZ = 20 * cm;

        // detector materials
        G4Material* lead = nist->FindOrBuildMaterial("G4_Pb");
        G4Material* nickel = nist->FindOrBuildMaterial("G4_Ni");
        G4Material* copper = nist->FindOrBuildMaterial("G4_Cu");
        G4Material* gold = nist->FindOrBuildMaterial("G4_Au");
        G4Material* aluminium = nist->FindOrBuildMaterial("G4_Al");
        G4Material* iron = nist->FindOrBuildMaterial("G4_Fe");
        G4Material* magnesium = nist->FindOrBuildMaterial("G4_Mg");

        G4Box* solidDetector = new G4Box(
            "Detector",
            detectorSizeXY,
            detectorSizeXY,
            detectorSizeZ);

        G4LogicalVolume* logicDetector = new G4LogicalVolume(
            solidDetector,
            tungsten,
            "Detector");

        G4ThreeVector detectorPos = G4ThreeVector(0, 0, 35 * cm);
        G4RotationMatrix* detRotation = new G4RotationMatrix();


        // place the detector
        new G4PVPlacement(detRotation,
            detectorPos,
            logicDetector,
            "Detector",
            logicWorld,
            false,
            0);

        // Define this detector as the gamma detector
        fGammaDetector = logicDetector;
	
        return physWorld;
    }

}