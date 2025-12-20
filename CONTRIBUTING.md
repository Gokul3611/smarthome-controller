# Contributing to Smart Home Controller

Thank you for your interest in contributing to the Smart Home Controller project!

## 📋 Repository Structure

```
smarthome-controller/
├── backend/              # Google Apps Script backend
├── frontend/             # Flutter mobile application
├── firmware/             # ESP32 firmware
├── hardware/             # PCB designs and schematics
├── app/                  # Compiled APK files
└── docs/                 # Additional documentation
```

## 🔧 Development Setup

### Backend Development
- Navigate to `backend/google-apps-script/`
- See [backend/google-apps-script/README.md](backend/google-apps-script/README.md) for setup

### Frontend Development
- Navigate to `frontend/flutter project/demo/`
- Requires Flutter SDK 3.10+
- Run `flutter pub get` to install dependencies
- See [app/README.md](app/README.md) for build instructions

### Firmware Development
- Navigate to `firmware/main/`
- Requires Arduino IDE or PlatformIO
- ESP32 board support required
- See [README.md](README.md) for library dependencies

### Hardware Development
- Navigate to `hardware/`
- KiCad project files in `hardware/pcb/`
- Schematics in `hardware/schematics/`
- BOM in `hardware/bom/`

## 🎯 Contribution Guidelines

### Code Style
- Follow existing code style in each component
- Use meaningful variable and function names
- Add comments for complex logic
- Update documentation when making changes

### Testing
- Test firmware changes on actual hardware when possible
- Verify backend changes in Google Apps Script environment
- Test mobile app on multiple Android versions
- Check hardware changes with KiCad ERC/DRC

### Documentation
- Update README.md if adding new features
- Add entries to CHANGELOG.md for significant changes
- Update relevant guides in the documentation

### Commit Messages
- Use clear, descriptive commit messages
- Reference issue numbers when applicable
- Group related changes in single commits

## 🚀 Pull Request Process

1. Fork the repository
2. Create a feature branch (`git checkout -b feature/amazing-feature`)
3. Make your changes
4. Test thoroughly
5. Update documentation
6. Commit your changes (`git commit -m 'Add amazing feature'`)
7. Push to your branch (`git push origin feature/amazing-feature`)
8. Open a Pull Request

## 📝 Reporting Issues

When reporting issues, please include:
- Clear description of the problem
- Steps to reproduce
- Expected vs actual behavior
- Hardware/software versions
- Error logs or screenshots

## 🔐 Security

If you discover a security vulnerability, please email the maintainers privately rather than opening a public issue.

## 📄 License

By contributing, you agree that your contributions will be licensed under the MIT License.

## 💬 Questions?

- Open an issue for feature requests or bugs
- Check existing documentation first
- Be respectful and constructive

Thank you for contributing! 🎉
