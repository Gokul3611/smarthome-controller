import 'dart:ui';
import 'package:flutter/material.dart';

class FanPage extends StatefulWidget {
  const FanPage({super.key});

  @override
  State<FanPage> createState() => _FanPageState();
}

class _FanPageState extends State<FanPage> {
  int speed = 0;
  bool isAuto = false;
  bool isRunning = false;

  void setSpeed(int value) {
    setState(() {
      speed = value;
      isRunning = value > 0;
      isAuto = false;
    });
  }

  void startFan() {
    setState(() {
      isRunning = true;
      if (speed == 0) speed = 1;
    });
  }

  void stopFan() {
    setState(() {
      isRunning = false;
      speed = 0;
      isAuto = false;
    });
  }

  void enableAutoMode() {
    setState(() {
      isAuto = true;
      isRunning = true;
      speed = 2;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          // Background same as login UI
          Container(
            decoration: const BoxDecoration(
              gradient: LinearGradient(
                begin: Alignment.topLeft,
                end: Alignment.bottomRight,
                colors: [Color(0xFF1C1C1E), Color(0xFF2C2C2E)],
              ),
            ),
          ),

          SafeArea(
            child: Padding(
              padding: const EdgeInsets.all(20),
              child: Column(
                children: [
                  // Glass AppBar style
                  ClipRRect(
                    borderRadius: BorderRadius.circular(20),
                    child: BackdropFilter(
                      filter: ImageFilter.blur(sigmaX: 20, sigmaY: 20),
                      child: Container(
                        width: double.infinity,
                        padding: const EdgeInsets.all(20),
                        decoration: BoxDecoration(
                          color: Colors.white.withOpacity(0.08),
                          borderRadius: BorderRadius.circular(20),
                          border: Border.all(
                            color: Colors.white.withOpacity(0.2),
                            width: 1.5,
                          ),
                        ),
                        child: const Text(
                          "Fan Control",
                          textAlign: TextAlign.center,
                          style: TextStyle(
                            fontSize: 26,
                            fontWeight: FontWeight.bold,
                            color: Colors.white,
                          ),
                        ),
                      ),
                    ),
                  ),

                  const SizedBox(height: 30),

                  // Fan Speed Section
                  _glassSection(
                    title: "Fan Speed",
                    child: Wrap(
                      spacing: 12,
                      children: [
                        for (int i = 1; i <= 4; i++)
                          ElevatedButton(
                            onPressed: () => setSpeed(i),
                            style: ElevatedButton.styleFrom(
                              backgroundColor: speed == i
                                  ? Colors.white.withOpacity(0.35)
                                  : Colors.white.withOpacity(0.15),
                              foregroundColor: Colors.white,
                              padding: const EdgeInsets.symmetric(
                                  horizontal: 20, vertical: 12),
                            ),
                            child: Text("Speed $i"),
                          ),
                      ],
                    ),
                  ),

                  const SizedBox(height: 30),

                  // Controls Section
                  _glassSection(
                    title: "Controls",
                    child: Row(
                      mainAxisAlignment: MainAxisAlignment.center,
                      children: [
                        ElevatedButton(
                          onPressed: startFan,
                          style: ElevatedButton.styleFrom(
                            backgroundColor: Colors.white.withOpacity(0.25),
                            foregroundColor: Colors.white,
                          ),
                          child: const Text("Start"),
                        ),
                        const SizedBox(width: 20),
                        ElevatedButton(
                          onPressed: stopFan,
                          style: ElevatedButton.styleFrom(
                            backgroundColor: Colors.white.withOpacity(0.25),
                            foregroundColor: Colors.white,
                          ),
                          child: const Text("Stop"),
                        ),
                      ],
                    ),
                  ),

                  const SizedBox(height: 30),

                  // Auto Mode button
                  SizedBox(
                    width: double.infinity,
                    child: ElevatedButton(
                      onPressed: enableAutoMode,
                      style: ElevatedButton.styleFrom(
                        backgroundColor: isAuto
                            ? Colors.green.withOpacity(0.4)
                            : Colors.white.withOpacity(0.15),
                        foregroundColor: Colors.white,
                        padding: const EdgeInsets.symmetric(
                            horizontal: 40, vertical: 16),
                      ),
                      child: const Text("Auto Mode"),
                    ),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  // Reusable glass container UI block
  Widget _glassSection({required String title, required Widget child}) {
    return ClipRRect(
      borderRadius: BorderRadius.circular(20),
      child: BackdropFilter(
        filter: ImageFilter.blur(sigmaX: 20, sigmaY: 20),
        child: Container(
          width: double.infinity,
          padding: const EdgeInsets.all(18),
          decoration: BoxDecoration(
            color: Colors.white.withOpacity(0.08),
            borderRadius: BorderRadius.circular(20),
            border: Border.all(
              color: Colors.white.withOpacity(0.2),
              width: 1.5,
            ),
          ),
          child: Column(
            children: [
              Text(
                title,
                style: const TextStyle(
                  fontSize: 22,
                  fontWeight: FontWeight.bold,
                  color: Colors.white,
                ),
              ),
              const SizedBox(height: 20),
              child,
            ],
          ),
        ),
      ),
    );
  }
}
