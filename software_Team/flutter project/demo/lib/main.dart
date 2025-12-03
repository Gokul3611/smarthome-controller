import 'package:flutter/material.dart';
import 'pages/login_screen.dart';
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
      title: 'HA App',
      debugShowCheckedModeBanner: false,
      theme: ThemeData(
        primarySwatch: Colors.red,
        useMaterial3: false,
      ),
      initialRoute: '/',
      routes: {
        '/': (_) => const LoginScreen(),
        '/home': (_) => const HomePage(),
        '/devices': (_) => const DevicesPage(),
        '/connection': (_) => const ConnectionPage(),
        '/profile': (_) => const ProfilePage(),
        '/faq': (_) => const FaqPage(),
      },
    );
  }
}
