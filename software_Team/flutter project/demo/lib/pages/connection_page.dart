import 'package:flutter/material.dart';
import 'package:connectivity_plus/connectivity_plus.dart';
import 'package:network_info_plus/network_info_plus.dart';

class ConnectionPage extends StatefulWidget {
  const ConnectionPage({super.key});

  @override
  State<ConnectionPage> createState() => _ConnectionPageState();
}

class _ConnectionPageState extends State<ConnectionPage> {
  String? ssid;
  String? bssid;
  String? ipv4;
  String? ipv6;
  String? connectionType;

  @override
  void initState() {
    super.initState();
    fetchNetworkInfo();
  }

  Future<void> fetchNetworkInfo() async {
    final info = NetworkInfo();
    final connectivity = Connectivity();

    var type = await connectivity.checkConnectivity();
    String netType = "None";

    if (type == ConnectivityResult.wifi) netType = "Wi-Fi";
    if (type == ConnectivityResult.mobile) netType = "Mobile";

    String? wifiSSID = await info.getWifiName();
    String? wifiBSSID = await info.getWifiBSSID();
    String? wifiIPv4 = await info.getWifiIP();
    String? wifiIPv6 = await info.getWifiIPv6();

    setState(() {
      connectionType = netType;
      ssid = wifiSSID;
      bssid = wifiBSSID;
      ipv4 = wifiIPv4;
      ipv6 = wifiIPv6;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text("Connection Details"),
        backgroundColor: Colors.black87,
      ),
      body: Padding(
        padding: const EdgeInsets.all(18.0),
        child: Column(
          crossAxisAlignment: CrossAxisAlignment.start,
          children: [
            buildItem("Connection Type", connectionType),
            buildItem("SSID", ssid),
            buildItem("BSSID", bssid),
            buildItem("IPv4 Address", ipv4),
            buildItem("IPv6 Address", ipv6),
          ],
        ),
      ),
    );
  }

  Widget buildItem(String title, String? value) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 14),
      child: Text(
        "$title: ${value ?? '—'}",
        style: const TextStyle(fontSize: 18, fontWeight: FontWeight.w500),
      ),
    );
  }
}
