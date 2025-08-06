# Contributing to ESP32 OBD2 Analog Gauges

Thank you for your interest in contributing to this project! We welcome contributions from the community and are pleased to have you join us.

## 🤝 How to Contribute

### Reporting Bugs
Before creating bug reports, please check the existing issues to avoid duplicates. When creating a bug report, include:

- **Clear description** of the issue
- **Steps to reproduce** the behavior
- **Expected behavior** vs actual behavior
- **Hardware setup** (ESP32 board, display model, OBD2 adapter)
- **Software versions** (PlatformIO, Arduino IDE, library versions)
- **Serial output** or error messages
- **Photos/videos** if applicable

### Suggesting Features
Feature requests are welcome! Please provide:

- **Clear description** of the feature
- **Use case** and motivation
- **Proposed implementation** (if you have ideas)
- **Compatibility considerations** with existing hardware

### Code Contributions

#### Development Setup
1. **Fork** the repository
2. **Clone** your fork locally
3. **Install** PlatformIO or Arduino IDE
4. **Set up** hardware for testing
5. **Create** a feature branch

#### Coding Standards
- **Code Style**: Follow existing code formatting
- **Comments**: Document complex functions and algorithms
- **Naming**: Use descriptive variable and function names
- **Constants**: Use `#define` for configuration values
- **Error Handling**: Include proper error checking

#### Code Review Process
1. **Create** a pull request with clear description
2. **Include** tests if applicable
3. **Update** documentation if needed
4. **Respond** to review feedback promptly
5. **Squash** commits before merging

## 🔧 Development Guidelines

### Hardware Testing
- Test on actual ESP32 hardware
- Verify with real OBD2 connection when possible
- Use display test mode for development without vehicle
- Test button functionality thoroughly

### Software Testing
- Compile successfully on PlatformIO
- Test both gauge and diagnostic modes
- Verify Bluetooth connectivity
- Check memory usage and performance

### Documentation
- Update README.md for new features
- Add inline code comments
- Update configuration documentation
- Include wiring diagrams for hardware changes

## 📋 Pull Request Checklist

Before submitting a pull request, ensure:

- [ ] Code compiles without errors or warnings
- [ ] Feature works on actual hardware
- [ ] Documentation is updated
- [ ] Commit messages are clear and descriptive
- [ ] No unnecessary files are included
- [ ] Code follows existing style conventions
- [ ] New features are configurable when appropriate

## 🏷️ Commit Message Format

Use clear, descriptive commit messages:

```
type: Brief description

Detailed explanation if needed

- Specific changes made
- Why the changes were necessary
- Any breaking changes or migration notes
```

**Types:**
- `feat:` New feature
- `fix:` Bug fix
- `docs:` Documentation changes
- `style:` Code formatting changes
- `refactor:` Code restructuring
- `test:` Adding or updating tests
- `chore:` Maintenance tasks

## 🎯 Areas for Contribution

### High Priority
- **Additional OBD2 PIDs**: Support for more vehicle parameters
- **Error Handling**: Improved connection recovery
- **Performance**: Memory optimization and faster updates
- **Documentation**: More detailed setup guides

### Medium Priority
- **Data Logging**: SD card storage for diagnostic data
- **Custom Gauges**: User-configurable gauge layouts
- **WiFi Connectivity**: Alternative to Bluetooth
- **Mobile App**: Companion smartphone application

### Low Priority
- **3D Models**: Enclosure designs for 3D printing
- **PCB Design**: Custom circuit board layouts
- **Advanced Features**: Graphing, data analysis
- **Multi-language**: Internationalization support

## 🚫 What We Don't Accept

- **Proprietary code** or copyrighted material
- **Breaking changes** without discussion
- **Untested code** that doesn't compile
- **Large refactors** without prior approval
- **Commercial advertisements** or spam

## 📞 Getting Help

If you need help with development:

- **GitHub Discussions**: For general questions
- **GitHub Issues**: For specific problems
- **Email**: andreas.auer03@gmail.com for complex topics

## 🏆 Recognition

Contributors will be:
- **Listed** in the project README
- **Credited** in release notes
- **Thanked** in commit messages
- **Invited** to be maintainers for significant contributions

## 📄 License Agreement

By contributing to this project, you agree that your contributions will be licensed under the MIT License.

---

Thank you for contributing to the ESP32 OBD2 Analog Gauges project! 🚗⚡
