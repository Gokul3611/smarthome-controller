import 'package:flutter/material.dart';
import 'config/app_theme.dart';
import 'pages/login_screen.dart';
import 'pages/dashboard_page.dart';
import 'pages/home_page.dart';
import 'pages/devices_page.dart';
import 'pages/connection_page.dart';
import 'pages/profile_page.dart';
import 'pages/faq_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Smart Home Controller',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primaryColor: AppTheme.primaryIndigo,
        scaffoldBackgroundColor: AppTheme.backgroundDark,
        useMaterial3: true,
        colorScheme: ColorScheme.dark(
          primary: AppTheme.primaryIndigo,
          secondary: AppTheme.primaryPurple,
          surface: AppTheme.backgroundMedium,
          background: AppTheme.backgroundDark,
        ),
        textTheme: const TextTheme(
          displayLarge: AppTheme.headingLarge,
          displayMedium: AppTheme.headingMedium,
          bodyLarge: AppTheme.bodyLarge,
          bodyMedium: AppTheme.bodyMedium,
          bodySmall: AppTheme.caption,
        ),
      ),
      initialRoute: '/',
      routes: {
        '/': (_) => const LoginScreen(),
        '/dashboard': (_) => const DashboardPage(),
        '/home': (_) => const HomePage(),
        '/devices': (_) => const DevicesPage(),
        '/connection': (_) => const ConnectionPage(),
        '/profile': (_) => const ProfilePage(),
        '/faq': (_) => const FaqPage(),
      },
    );
  }
}
