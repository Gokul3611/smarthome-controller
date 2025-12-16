# System Architecture and Structure

**Document Number:** SYS-STRUCT-001  
**Revision:** 1.0  
**Date:** 2025-12-16  
**Classification:** Technical Specification

Complete repository organization for easy navigation.

## 1.0 Directory Structure

```
smarthome-controller/

  app/                          # Mobile App Distribution
    release/                    # Release APKs
    debug/                      # Debug APKs
    README.md                   # App documentation
    QUICK_START.md              # Quick build guide

  backend/                      # Cloud Backend
    google-apps-script/
        Code.gs                 # Main API logic
        Database.gs             # DB helper functions
        Dashboard.html          # Web dashboard UI
        appsscript.json         # Manifest
        README.md               # Backend docs & deployment

  firmware/                     # ESP32 Firmware
    main/
        main.ino                # Main firmware file
        config.h                # Configuration
        api.h/api_impl.h        # API implementation
        voice.h/voice_impl.h    # Voice assistant

  hardware/                     # Hardware Documentation
    README.md                   # Hardware overview
    CONNECTION_GUIDE.md         # Wiring instructions
    bom/
       BOM.md                  # Bill of materials
    schematics/                 # Circuit diagrams
       README.md
    pcb/                        # PCB designs

  software_Team/                # Software Development
    flutter project/demo/      # Flutter mobile app
        lib/
           config/             # API configuration
           models/             # Data models
           services/           # API services
           pages/              # UI pages
           main.dart
        android/                # Android config
        ios/                    # iOS config
        pubspec.yaml            # Dependencies

  docs/                         # Additional Documentation
    archive/                    # Legacy files

  Root Documentation Files
    README.md                   #  Main project overview
    FEATURES.md                 # Complete feature list (100+)
    DEPLOYMENT.md               # 30-min setup guide
    PRODUCTION_GUIDE.md         # Production deployment
    OTA_GUIDE.md               # Firmware updates
    HANDOVER.md                # Company transfer
    IMPLEMENTATION_SUMMARY.md   # Technical details
    INSTALL.md                 # Installation guide
    CHANGELOG.md               # Version history
    PROJECT_STRUCTURE.md       # This file

  Configuration Files
     .gitignore                  # Git ignore rules
     _codeql_detected_source_root # CodeQL config
```



## 2.0 Documentation Guide

### 2.1 Getting Started
1. **First Time?** → Read `README.md`
2. **Want to Deploy?** → Follow `DEPLOYMENT.md`
3. **See Features?** → Check `FEATURES.md`

### 2.2 For Developers
- **Hardware Design** → `hardware/README.md`
- **Backend API** → `backend/google-apps-script/README.md`
- **Mobile App** → `software_Team/flutter project/demo/`
- **Firmware** → `firmware/main/`

### 2.3 For Production
- **Deploy to Production** → `PRODUCTION_GUIDE.md`
- **Setup OTA Updates** → `OTA_GUIDE.md`
- **Hardware Assembly** → `hardware/CONNECTION_GUIDE.md`
- **Bill of Materials** → `hardware/bom/BOM.md`

### 2.4 For Company Handover
- **Transfer Document** → `HANDOVER.md`
- **All Deliverables** → Listed in `HANDOVER.md`
- **Access Guide** → Section in `HANDOVER.md`



## 3.0 Quick Access by Role

### 3.1 Hardware Engineer
```
hardware/
 README.md              # Start here
 CONNECTION_GUIDE.md    # Assembly instructions
 bom/BOM.md            # Parts list
```

### 3.2 Firmware Developer
```
firmware/main/
 main.ino              # Main code
 config.h              # Configuration
 See: OTA_GUIDE.md     # For updates
```

### 3.3 Backend Developer
```
backend/google-apps-script/
 Code.gs               # API endpoints
 Database.gs           # DB functions
 README.md            # Deployment
```

### 3.4 Mobile App Developer
```
software_Team/flutter project/demo/lib/
 config/               # API settings
 models/               # Data models
 services/             # API layer
 pages/                # UI screens
```

### 3.5 Project Manager
```
Root Level:
 README.md             # Overview
 FEATURES.md           # What's built
 PRODUCTION_GUIDE.md   # How to deploy
 HANDOVER.md          # Transfer info
```

### 3.6 DevOps Engineer
```
Key Files:
 DEPLOYMENT.md         # Setup guide
 PRODUCTION_GUIDE.md   # Production deploy
 OTA_GUIDE.md         # Update process
 backend/.../README.md # Backend deploy
```



## 4.0 File Statistics

| Category | Count | Total Size |
|----------|-------|------------|
| Documentation (MD) | 18 files | ~90 KB |
| Source Code (ino, gs, dart) | 50+ files | ~200 KB |
| Configuration | 10+ files | ~50 KB |
| **Total Documentation** | **18 files** | **50,000+ words** |



## 5.0 Finding What You Need

### 5.1 Search by Topic

**API & Backend**
- `backend/google-apps-script/README.md`
- `PRODUCTION_GUIDE.md` (API deployment)

**Hardware**
- `hardware/README.md` (overview)
- `hardware/CONNECTION_GUIDE.md` (wiring)
- `hardware/bom/BOM.md` (parts)

**Mobile App**
- `app/README.md` (APK build)
- `software_Team/flutter project/demo/README.md`

**Deployment**
- `DEPLOYMENT.md` (quick start)
- `PRODUCTION_GUIDE.md` (full production)
- `HANDOVER.md` (company transfer)

**Updates**
- `OTA_GUIDE.md` (firmware updates)
- `CHANGELOG.md` (version history)

**Features**
- `FEATURES.md` (complete list)
- `IMPLEMENTATION_SUMMARY.md` (technical)



## 6.0 Project Organization Philosophy

### 6.1 Principles

1. **Separation of Concerns**
   - Hardware, firmware, backend, and app are separate
   - Clear boundaries between components

2. **Documentation First**
   - Every folder has README.md
   - Comprehensive guides for all processes

3. **Production Ready**
   - Not just code, but complete system
   - Deployment, testing, maintenance covered

4. **Professional Structure**
   - Industry-standard organization
   - Easy for teams to navigate
   - Interview/portfolio ready

5. **Scalable Design**
   - Easy to add new components
   - Clear patterns established
   - Future-proof architecture



## 7.0 Common Tasks

### 7.1 Build APK
```bash
cd "software_Team/flutter project/demo"
flutter build apk --release
# See: app/QUICK_START.md
```

### 7.2 Deploy Backend
```
1. Open backend/google-apps-script/README.md
2. Follow deployment steps
3. Get Web App URL
4. Update configs
```

### 7.3 Flash Firmware
```
1. Open firmware/main/main.ino in Arduino IDE
2. Configure settings in config.h
3. Upload to ESP32
# See: DEPLOYMENT.md
```

### 7.4 Assemble Hardware
```
1. Read hardware/CONNECTION_GUIDE.md
2. Follow safety warnings
3. Check hardware/bom/BOM.md for parts
4. Test as described
```



## 8.0 Support

### 8.1 For Questions About:

- **Project Structure**: This file
- **Specific Component**: See that folder's README.md
- **Deployment**: DEPLOYMENT.md or PRODUCTION_GUIDE.md
- **Company Transfer**: HANDOVER.md

### 8.2 Can't Find Something?

1. Check this file's Table of Contents
2. Look in relevant folder's README.md
3. Search repo for keywords
4. Check main README.md navigation



## 9.0 Checklist for New Team Members

- [ ] Read main `README.md`
- [ ] Review `FEATURES.md` to understand what's built
- [ ] Check `PROJECT_STRUCTURE.md` (this file)
- [ ] Browse relevant component folders
- [ ] Follow `DEPLOYMENT.md` to set up dev environment
- [ ] Review `HANDOVER.md` for context
- [ ] Ask questions via GitHub Issues



## 10.0 Maintaining This Structure

### 10.1 Adding New Components

1. Create folder in appropriate location
2. Add README.md to new folder
3. Update this file with new structure
4. Update main README.md navigation
5. Cross-reference in related docs

### 10.2 Moving Files

1. Update all references in documentation
2. Update this structure file
3. Test all links
4. Commit with clear message



**Last Updated**: December 2024  
**Version**: 1.0  
**Status**: Complete



*This structure represents a complete, production-ready IoT system with professional organization suitable for company use and portfolio presentation.*
