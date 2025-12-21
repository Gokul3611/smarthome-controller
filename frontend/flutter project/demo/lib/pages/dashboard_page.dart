import 'dart:async';
import 'dart:ui';
import 'package:flutter/material.dart';
import '../config/app_theme.dart';
import '../models/device.dart';
import '../models/schedule.dart';
import '../services/api_service.dart';

class DashboardPage extends StatefulWidget {
  final String userId;

  const DashboardPage({super.key, this.userId = 'default'});

  @override
  State<DashboardPage> createState() => _DashboardPageState();
}

class _DashboardPageState extends State<DashboardPage>
    with SingleTickerProviderStateMixin {
  final ApiService _apiService = ApiService();
  List<Device> devices = [];
  List<Schedule> schedules = [];
  List<Scene> scenes = [];
  bool isLoading = false;
  bool isConnected = true;
  Timer? _refreshTimer;
  late TabController _tabController;
  int _selectedTabIndex = 0;

  @override
  void initState() {
    super.initState();
    _tabController = TabController(length: 3, vsync: this);
    _tabController.addListener(() {
      setState(() {
        _selectedTabIndex = _tabController.index;
      });
      if (_selectedTabIndex == 1) {
        _loadSchedules();
      } else if (_selectedTabIndex == 2) {
        _loadScenes();
      }
    });
    _loadAllData();
    _startAutoRefresh();
  }

  @override
  void dispose() {
    _refreshTimer?.cancel();
    _tabController.dispose();
    super.dispose();
  }

  void _startAutoRefresh() {
    _refreshTimer = Timer.periodic(const Duration(seconds: 3), (timer) {
      if (_selectedTabIndex == 0) {
        _loadDevices();
      }
    });
  }

  Future<void> _loadAllData() async {
    setState(() {
      isLoading = true;
    });

    await Future.wait([
      _loadDevices(),
      _loadSchedules(),
      _loadScenes(),
    ]);

    setState(() {
      isLoading = false;
    });
  }

  Future<void> _loadDevices() async {
    try {
      final loadedDevices = await _apiService.getDevices(widget.userId);
      if (mounted) {
        setState(() {
          devices = loadedDevices;
          isConnected = true;
        });
      }
    } catch (e) {
      if (mounted) {
        setState(() {
          isConnected = false;
        });
      }
    }
  }

  Future<void> _loadSchedules() async {
    try {
      final loadedSchedules = await _apiService.getSchedules(widget.userId);
      if (mounted) {
        setState(() {
          schedules = loadedSchedules;
        });
      }
    } catch (e) {
      print('Error loading schedules: $e');
    }
  }

  Future<void> _loadScenes() async {
    try {
      final loadedScenes = await _apiService.getScenes(widget.userId);
      if (mounted) {
        setState(() {
          scenes = loadedScenes;
        });
      }
    } catch (e) {
      print('Error loading scenes: $e');
    }
  }

  int get activeDevicesCount {
    return devices.where((d) => d.power && d.online).length;
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          // Background gradient
          Container(
            decoration: BoxDecoration(
              gradient: AppTheme.backgroundGradient,
            ),
          ),

          // Main content
          SafeArea(
            child: Padding(
              padding: const EdgeInsets.all(20),
              child: Column(
                children: [
                  // Header with gradient
                  _buildHeader(),
                  const SizedBox(height: 20),

                  // System statistics
                  _buildSystemStats(),
                  const SizedBox(height: 24),

                  // Tabs
                  _buildTabs(),
                  const SizedBox(height: 20),

                  // Tab content
                  Expanded(
                    child: _buildTabContent(),
                  ),
                ],
              ),
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildHeader() {
    return ClipRRect(
      borderRadius: BorderRadius.circular(24),
      child: BackdropFilter(
        filter: ImageFilter.blur(sigmaX: 24, sigmaY: 24),
        child: Container(
          padding: const EdgeInsets.all(24),
          decoration: AppTheme.glassContainer(),
          child: Column(
            children: [
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Row(
                    children: [
                      Container(
                        width: 56,
                        height: 56,
                        decoration: BoxDecoration(
                          gradient: AppTheme.primaryGradient,
                          borderRadius: BorderRadius.circular(16),
                          boxShadow: [
                            BoxShadow(
                              color: AppTheme.primaryIndigo.withOpacity(0.4),
                              blurRadius: 16,
                              offset: const Offset(0, 4),
                            ),
                          ],
                        ),
                        child: const Icon(
                          Icons.home_rounded,
                          color: Colors.white,
                          size: 32,
                        ),
                      ),
                      const SizedBox(width: 16),
                      Column(
                        crossAxisAlignment: CrossAxisAlignment.start,
                        children: [
                          Text(
                            'Smart Home Controller',
                            style: AppTheme.headingMedium.copyWith(
                              fontSize: 24,
                              fontWeight: FontWeight.w700,
                            ),
                          ),
                          const SizedBox(height: 4),
                          Row(
                            children: [
                              const Text(
                                'Professional IoT System',
                                style: TextStyle(
                                  color: AppTheme.textSecondary,
                                  fontSize: 12,
                                ),
                              ),
                              const SizedBox(width: 8),
                              _buildConnectionStatus(),
                            ],
                          ),
                        ],
                      ),
                    ],
                  ),
                  IconButton(
                    icon: const Icon(Icons.settings_rounded),
                    color: Colors.white70,
                    onPressed: _showSettingsDialog,
                  ),
                ],
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildConnectionStatus() {
    return Container(
      padding: const EdgeInsets.symmetric(horizontal: 10, vertical: 4),
      decoration: BoxDecoration(
        color: isConnected
            ? AppTheme.successGreen.withOpacity(0.15)
            : AppTheme.errorRed.withOpacity(0.15),
        border: Border.all(
          color: isConnected
              ? AppTheme.successGreen.withOpacity(0.3)
              : AppTheme.errorRed.withOpacity(0.3),
        ),
        borderRadius: BorderRadius.circular(8),
      ),
      child: Row(
        mainAxisSize: MainAxisSize.min,
        children: [
          Container(
            width: 6,
            height: 6,
            decoration: BoxDecoration(
              color: isConnected ? AppTheme.successGreen : AppTheme.errorRed,
              shape: BoxShape.circle,
            ),
          ),
          const SizedBox(width: 6),
          Text(
            isConnected ? 'Connected' : 'Disconnected',
            style: TextStyle(
              color: isConnected ? AppTheme.successGreen : AppTheme.errorRed,
              fontSize: 11,
              fontWeight: FontWeight.w600,
            ),
          ),
        ],
      ),
    );
  }

  Widget _buildSystemStats() {
    return Row(
      children: [
        Expanded(
          child: _buildStatCard(
            icon: Icons.devices_rounded,
            label: 'TOTAL DEVICES',
            value: devices.length.toString(),
            gradient: LinearGradient(
              colors: [
                AppTheme.successGreen,
                AppTheme.successGreen.withOpacity(0.7),
              ],
            ),
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: _buildStatCard(
            icon: Icons.flash_on_rounded,
            label: 'ACTIVE',
            value: activeDevicesCount.toString(),
            gradient: LinearGradient(
              colors: [
                Colors.blue,
                Colors.blue.withOpacity(0.7),
              ],
            ),
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: _buildStatCard(
            icon: Icons.schedule_rounded,
            label: 'SCHEDULES',
            value: schedules.length.toString(),
            gradient: LinearGradient(
              colors: [
                AppTheme.primaryPurple,
                AppTheme.primaryPurple.withOpacity(0.7),
              ],
            ),
          ),
        ),
        const SizedBox(width: 12),
        Expanded(
          child: _buildStatCard(
            icon: Icons.movie_rounded,
            label: 'SCENES',
            value: scenes.length.toString(),
            gradient: LinearGradient(
              colors: [
                Colors.orange,
                Colors.orange.withOpacity(0.7),
              ],
            ),
          ),
        ),
      ],
    );
  }

  Widget _buildStatCard({
    required IconData icon,
    required String label,
    required String value,
    required Gradient gradient,
  }) {
    return ClipRRect(
      borderRadius: BorderRadius.circular(16),
      child: BackdropFilter(
        filter: ImageFilter.blur(sigmaX: 16, sigmaY: 16),
        child: Container(
          padding: const EdgeInsets.all(16),
          decoration: BoxDecoration(
            color: Colors.white.withOpacity(0.05),
            borderRadius: BorderRadius.circular(16),
            border: Border.all(
              color: Colors.white.withOpacity(0.1),
            ),
          ),
          child: Column(
            children: [
              Container(
                width: 40,
                height: 40,
                decoration: BoxDecoration(
                  gradient: gradient,
                  borderRadius: BorderRadius.circular(10),
                ),
                child: Icon(icon, color: Colors.white, size: 20),
              ),
              const SizedBox(height: 8),
              Text(
                label,
                style: const TextStyle(
                  color: AppTheme.textSecondary,
                  fontSize: 9,
                  fontWeight: FontWeight.w600,
                  letterSpacing: 0.5,
                ),
              ),
              const SizedBox(height: 4),
              Text(
                value,
                style: const TextStyle(
                  color: Colors.white,
                  fontSize: 20,
                  fontWeight: FontWeight.w700,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildTabs() {
    return Container(
      padding: const EdgeInsets.all(6),
      decoration: BoxDecoration(
        color: Colors.white.withOpacity(0.03),
        borderRadius: BorderRadius.circular(16),
        border: Border.all(
          color: Colors.white.withOpacity(0.08),
        ),
      ),
      child: TabBar(
        controller: _tabController,
        indicator: BoxDecoration(
          gradient: AppTheme.primaryGradient,
          borderRadius: BorderRadius.circular(12),
          boxShadow: [
            BoxShadow(
              color: AppTheme.primaryIndigo.withOpacity(0.3),
              blurRadius: 12,
              offset: const Offset(0, 4),
            ),
          ],
        ),
        indicatorSize: TabBarIndicatorSize.tab,
        labelColor: Colors.white,
        unselectedLabelColor: AppTheme.textSecondary,
        labelStyle: const TextStyle(
          fontSize: 14,
          fontWeight: FontWeight.w600,
        ),
        tabs: const [
          Tab(text: 'Devices'),
          Tab(text: 'Schedules'),
          Tab(text: 'Scenes'),
        ],
      ),
    );
  }

  Widget _buildTabContent() {
    return TabBarView(
      controller: _tabController,
      children: [
        _buildDevicesTab(),
        _buildSchedulesTab(),
        _buildScenesTab(),
      ],
    );
  }

  Widget _buildDevicesTab() {
    if (isLoading && devices.isEmpty) {
      return const Center(
        child: CircularProgressIndicator(
          color: AppTheme.primaryIndigo,
        ),
      );
    }

    if (devices.isEmpty) {
      return _buildEmptyState(
        icon: Icons.devices_rounded,
        title: 'No Devices Found',
        subtitle: 'Connect your ESP32 devices to get started',
      );
    }

    return RefreshIndicator(
      onRefresh: _loadDevices,
      color: AppTheme.primaryIndigo,
      child: GridView.builder(
        padding: const EdgeInsets.only(bottom: 20),
        gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
          crossAxisCount: 1,
          childAspectRatio: 1.3,
          mainAxisSpacing: 16,
        ),
        itemCount: devices.length,
        itemBuilder: (context, index) => _buildDeviceCard(devices[index]),
      ),
    );
  }

  Widget _buildDeviceCard(Device device) {
    return ClipRRect(
      borderRadius: BorderRadius.circular(20),
      child: BackdropFilter(
        filter: ImageFilter.blur(sigmaX: 24, sigmaY: 24),
        child: Container(
          padding: const EdgeInsets.all(20),
          decoration: BoxDecoration(
            gradient: LinearGradient(
              begin: Alignment.topLeft,
              end: Alignment.bottomRight,
              colors: [
                Colors.white.withOpacity(0.05),
                Colors.white.withOpacity(0.02),
              ],
            ),
            borderRadius: BorderRadius.circular(20),
            border: Border.all(
              color: Colors.white.withOpacity(0.1),
            ),
          ),
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              // Device header
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Expanded(
                    child: Row(
                      children: [
                        Expanded(
                          child: InkWell(
                            onTap: () => _showDeviceEditDialog(device),
                            child: Column(
                              crossAxisAlignment: CrossAxisAlignment.start,
                              children: [
                                Row(
                                  children: [
                                    Flexible(
                                      child: Text(
                                        device.name,
                                        style: const TextStyle(
                                          color: Colors.white,
                                          fontSize: 20,
                                          fontWeight: FontWeight.w700,
                                        ),
                                        overflow: TextOverflow.ellipsis,
                                      ),
                                    ),
                                    const SizedBox(width: 8),
                                    const Icon(
                                      Icons.edit_rounded,
                                      size: 16,
                                      color: Colors.white38,
                                    ),
                                  ],
                                ),
                                const SizedBox(height: 4),
                                Text(
                                  device.uid,
                                  style: TextStyle(
                                    color: AppTheme.textSecondary,
                                    fontSize: 11,
                                    fontFamily: 'monospace',
                                  ),
                                  overflow: TextOverflow.ellipsis,
                                ),
                              ],
                            ),
                          ),
                        ),
                      ],
                    ),
                  ),
                  Container(
                    padding:
                        const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
                    decoration: BoxDecoration(
                      color: device.online
                          ? AppTheme.successGreen.withOpacity(0.15)
                          : AppTheme.errorRed.withOpacity(0.15),
                      border: Border.all(
                        color: device.online
                            ? AppTheme.successGreen.withOpacity(0.3)
                            : AppTheme.errorRed.withOpacity(0.3),
                      ),
                      borderRadius: BorderRadius.circular(8),
                    ),
                    child: Row(
                      mainAxisSize: MainAxisSize.min,
                      children: [
                        Container(
                          width: 8,
                          height: 8,
                          decoration: BoxDecoration(
                            color: device.online
                                ? AppTheme.successGreen
                                : AppTheme.errorRed,
                            shape: BoxShape.circle,
                          ),
                        ),
                        const SizedBox(width: 6),
                        Text(
                          device.online ? 'Online' : 'Offline',
                          style: TextStyle(
                            color: device.online
                                ? AppTheme.successGreen
                                : AppTheme.errorRed,
                            fontSize: 12,
                            fontWeight: FontWeight.w600,
                          ),
                        ),
                      ],
                    ),
                  ),
                ],
              ),

              const SizedBox(height: 12),

              // Device type badge
              Container(
                padding: const EdgeInsets.symmetric(horizontal: 12, vertical: 6),
                decoration: BoxDecoration(
                  color: AppTheme.primaryPurple.withOpacity(0.15),
                  border: Border.all(
                    color: AppTheme.primaryPurple.withOpacity(0.3),
                  ),
                  borderRadius: BorderRadius.circular(8),
                ),
                child: Row(
                  mainAxisSize: MainAxisSize.min,
                  children: [
                    Icon(
                      device.type == 'fan' ? Icons.toys_rounded : Icons.lightbulb_rounded,
                      size: 16,
                      color: AppTheme.primaryPurple.withOpacity(0.8),
                    ),
                    const SizedBox(width: 6),
                    Text(
                      device.type == 'fan' ? 'Fan' : 'Light',
                      style: TextStyle(
                        color: AppTheme.primaryPurple.withOpacity(0.8),
                        fontSize: 12,
                        fontWeight: FontWeight.w600,
                      ),
                    ),
                  ],
                ),
              ),

              const SizedBox(height: 16),

              // Power control
              Container(
                padding: const EdgeInsets.symmetric(vertical: 12),
                decoration: BoxDecoration(
                  border: Border(
                    bottom: BorderSide(
                      color: Colors.white.withOpacity(0.05),
                    ),
                  ),
                ),
                child: Row(
                  mainAxisAlignment: MainAxisAlignment.spaceBetween,
                  children: [
                    Row(
                      children: [
                        Icon(
                          Icons.flash_on_rounded,
                          size: 18,
                          color: Colors.white.withOpacity(0.8),
                        ),
                        const SizedBox(width: 8),
                        Text(
                          'Power',
                          style: TextStyle(
                            color: Colors.white.withOpacity(0.8),
                            fontSize: 14,
                            fontWeight: FontWeight.w500,
                          ),
                        ),
                      ],
                    ),
                    Switch(
                      value: device.power,
                      onChanged: device.online
                          ? (value) => _toggleDevice(device, value)
                          : null,
                      activeColor: AppTheme.successGreen,
                      activeTrackColor: AppTheme.successGreen.withOpacity(0.5),
                    ),
                  ],
                ),
              ),

              const SizedBox(height: 12),

              // Brightness/Speed control
              Row(
                mainAxisAlignment: MainAxisAlignment.spaceBetween,
                children: [
                  Row(
                    children: [
                      Icon(
                        device.type == 'fan'
                            ? Icons.wind_power_rounded
                            : Icons.brightness_6_rounded,
                        size: 18,
                        color: Colors.white.withOpacity(0.8),
                      ),
                      const SizedBox(width: 8),
                      Text(
                        device.type == 'fan' ? 'Speed' : 'Brightness',
                        style: TextStyle(
                          color: Colors.white.withOpacity(0.8),
                          fontSize: 14,
                          fontWeight: FontWeight.w500,
                        ),
                      ),
                    ],
                  ),
                  Text(
                    '${device.value}%',
                    style: TextStyle(
                      color: AppTheme.primaryIndigo.withOpacity(0.8),
                      fontSize: 14,
                      fontWeight: FontWeight.w600,
                    ),
                  ),
                ],
              ),

              const SizedBox(height: 8),

              SliderTheme(
                data: SliderThemeData(
                  activeTrackColor: AppTheme.primaryIndigo,
                  inactiveTrackColor: AppTheme.primaryIndigo.withOpacity(0.2),
                  thumbColor: AppTheme.primaryIndigo,
                  overlayColor: AppTheme.primaryIndigo.withOpacity(0.2),
                  thumbShape: const RoundSliderThumbShape(enabledThumbRadius: 10),
                ),
                child: Slider(
                  value: device.value.toDouble(),
                  min: 0,
                  max: 100,
                  divisions: 100,
                  onChanged: device.online && device.power
                      ? (value) {
                          setState(() {
                            device.value = value.toInt();
                          });
                        }
                      : null,
                  onChangeEnd: device.online && device.power
                      ? (value) => _updateDeviceValue(device, value.toInt())
                      : null,
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }

  Widget _buildSchedulesTab() {
    if (schedules.isEmpty) {
      return Column(
        children: [
          Expanded(
            child: _buildEmptyState(
              icon: Icons.schedule_rounded,
              title: 'No Schedules',
              subtitle: 'Create automated schedules for your devices',
            ),
          ),
          const SizedBox(height: 16),
          _buildActionButton(
            label: '+ Add Schedule',
            onPressed: _showAddScheduleDialog,
          ),
        ],
      );
    }

    return Column(
      children: [
        Expanded(
          child: ListView.builder(
            padding: const EdgeInsets.only(bottom: 20),
            itemCount: schedules.length,
            itemBuilder: (context, index) =>
                _buildScheduleCard(schedules[index]),
          ),
        ),
        const SizedBox(height: 16),
        _buildActionButton(
          label: '+ Add Schedule',
          onPressed: _showAddScheduleDialog,
        ),
      ],
    );
  }

  Widget _buildScheduleCard(Schedule schedule) {
    final device = devices.firstWhere(
      (d) => d.uid == schedule.deviceUid,
      orElse: () => Device(uid: '', name: 'Unknown', type: 'light'),
    );

    return Padding(
      padding: const EdgeInsets.only(bottom: 12),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(16),
        child: BackdropFilter(
          filter: ImageFilter.blur(sigmaX: 16, sigmaY: 16),
          child: Container(
            padding: const EdgeInsets.all(16),
            decoration: BoxDecoration(
              color: Colors.white.withOpacity(0.05),
              borderRadius: BorderRadius.circular(16),
              border: Border.all(
                color: Colors.white.withOpacity(0.1),
              ),
            ),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        schedule.name,
                        style: const TextStyle(
                          color: Colors.white,
                          fontSize: 16,
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                      const SizedBox(height: 4),
                      Text(
                        '${schedule.startTime} - ${schedule.endTime}',
                        style: const TextStyle(
                          color: AppTheme.textSecondary,
                          fontSize: 13,
                        ),
                      ),
                      if (device.name.isNotEmpty)
                        Text(
                          device.name,
                          style: TextStyle(
                            color: AppTheme.textSecondary.withOpacity(0.7),
                            fontSize: 12,
                          ),
                        ),
                    ],
                  ),
                ),
                IconButton(
                  icon: const Icon(Icons.delete_rounded),
                  color: AppTheme.errorRed,
                  onPressed: () => _deleteSchedule(schedule.id),
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildScenesTab() {
    if (scenes.isEmpty) {
      return Column(
        children: [
          Expanded(
            child: _buildEmptyState(
              icon: Icons.movie_rounded,
              title: 'No Scenes',
              subtitle: 'Create custom scenes for quick device control',
            ),
          ),
          const SizedBox(height: 16),
          _buildActionButton(
            label: '+ Add Scene',
            onPressed: _showAddSceneDialog,
          ),
        ],
      );
    }

    return Column(
      children: [
        Expanded(
          child: ListView.builder(
            padding: const EdgeInsets.only(bottom: 20),
            itemCount: scenes.length,
            itemBuilder: (context, index) => _buildSceneCard(scenes[index]),
          ),
        ),
        const SizedBox(height: 16),
        _buildActionButton(
          label: '+ Add Scene',
          onPressed: _showAddSceneDialog,
        ),
      ],
    );
  }

  Widget _buildSceneCard(Scene scene) {
    return Padding(
      padding: const EdgeInsets.only(bottom: 12),
      child: ClipRRect(
        borderRadius: BorderRadius.circular(16),
        child: BackdropFilter(
          filter: ImageFilter.blur(sigmaX: 16, sigmaY: 16),
          child: Container(
            padding: const EdgeInsets.all(16),
            decoration: BoxDecoration(
              color: Colors.white.withOpacity(0.05),
              borderRadius: BorderRadius.circular(16),
              border: Border.all(
                color: Colors.white.withOpacity(0.1),
              ),
            ),
            child: Row(
              mainAxisAlignment: MainAxisAlignment.spaceBetween,
              children: [
                Expanded(
                  child: Column(
                    crossAxisAlignment: CrossAxisAlignment.start,
                    children: [
                      Text(
                        scene.name,
                        style: const TextStyle(
                          color: Colors.white,
                          fontSize: 16,
                          fontWeight: FontWeight.w600,
                        ),
                      ),
                      const SizedBox(height: 4),
                      Text(
                        '${scene.devices.length} devices',
                        style: const TextStyle(
                          color: AppTheme.textSecondary,
                          fontSize: 13,
                        ),
                      ),
                    ],
                  ),
                ),
                Row(
                  children: [
                    ElevatedButton(
                      onPressed: () => _activateScene(scene.id),
                      style: ElevatedButton.styleFrom(
                        backgroundColor: AppTheme.successGreen,
                        foregroundColor: Colors.white,
                        padding: const EdgeInsets.symmetric(
                          horizontal: 16,
                          vertical: 8,
                        ),
                        shape: RoundedRectangleBorder(
                          borderRadius: BorderRadius.circular(8),
                        ),
                      ),
                      child: const Text('Activate'),
                    ),
                    const SizedBox(width: 8),
                    IconButton(
                      icon: const Icon(Icons.delete_rounded),
                      color: AppTheme.errorRed,
                      onPressed: () => _deleteScene(scene.id),
                    ),
                  ],
                ),
              ],
            ),
          ),
        ),
      ),
    );
  }

  Widget _buildEmptyState({
    required IconData icon,
    required String title,
    required String subtitle,
  }) {
    return Center(
      child: Column(
        mainAxisAlignment: MainAxisAlignment.center,
        children: [
          Icon(
            icon,
            size: 64,
            color: Colors.white24,
          ),
          const SizedBox(height: 16),
          Text(
            title,
            style: const TextStyle(
              color: AppTheme.textSecondary,
              fontSize: 20,
              fontWeight: FontWeight.w600,
            ),
          ),
          const SizedBox(height: 8),
          Text(
            subtitle,
            style: TextStyle(
              color: AppTheme.textSecondary.withOpacity(0.7),
              fontSize: 14,
            ),
            textAlign: TextAlign.center,
          ),
        ],
      ),
    );
  }

  Widget _buildActionButton({
    required String label,
    required VoidCallback onPressed,
  }) {
    return SizedBox(
      width: double.infinity,
      child: ElevatedButton(
        onPressed: onPressed,
        style: ElevatedButton.styleFrom(
          padding: const EdgeInsets.symmetric(vertical: 16),
          backgroundColor: AppTheme.primaryIndigo,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(12),
          ),
        ),
        child: Text(
          label,
          style: const TextStyle(
            fontSize: 16,
            fontWeight: FontWeight.w600,
          ),
        ),
      ),
    );
  }

  Future<void> _toggleDevice(Device device, bool value) async {
    setState(() {
      device.power = value;
    });

    final success = await _apiService.updateDeviceState(
      device.uid,
      0,
      value,
      device.value,
    );

    if (!success && mounted) {
      setState(() {
        device.power = !value;
      });
      _showSnackBar('Failed to update device');
    }
  }

  Future<void> _updateDeviceValue(Device device, int value) async {
    final success = await _apiService.updateDeviceState(
      device.uid,
      0,
      true,
      value,
    );

    if (!success && mounted) {
      _showSnackBar('Failed to update device');
    }
  }

  void _showDeviceEditDialog(Device device) {
    final nameController = TextEditingController(text: device.name);
    String selectedType = device.type;

    showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          backgroundColor: AppTheme.backgroundMedium,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20),
          ),
          title: const Row(
            children: [
              Icon(Icons.edit_rounded, color: AppTheme.primaryIndigo),
              SizedBox(width: 12),
              Text(
                'Edit Device',
                style: TextStyle(color: Colors.white),
              ),
            ],
          ),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              TextField(
                controller: nameController,
                style: const TextStyle(color: Colors.white),
                decoration: InputDecoration(
                  labelText: 'Device Name',
                  labelStyle: const TextStyle(color: AppTheme.textSecondary),
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: Colors.white.withOpacity(0.3),
                    ),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  focusedBorder: const OutlineInputBorder(
                    borderSide: BorderSide(color: AppTheme.primaryIndigo),
                    borderRadius: BorderRadius.all(Radius.circular(12)),
                  ),
                ),
                maxLength: 30,
              ),
              const SizedBox(height: 16),
              DropdownButtonFormField<String>(
                value: selectedType,
                dropdownColor: AppTheme.backgroundMedium,
                style: const TextStyle(color: Colors.white),
                decoration: InputDecoration(
                  labelText: 'Device Type',
                  labelStyle: const TextStyle(color: AppTheme.textSecondary),
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: Colors.white.withOpacity(0.3),
                    ),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  focusedBorder: const OutlineInputBorder(
                    borderSide: BorderSide(color: AppTheme.primaryIndigo),
                    borderRadius: BorderRadius.all(Radius.circular(12)),
                  ),
                ),
                items: const [
                  DropdownMenuItem(
                    value: 'light',
                    child: Row(
                      children: [
                        Icon(Icons.lightbulb_rounded, size: 20),
                        SizedBox(width: 8),
                        Text('Light'),
                      ],
                    ),
                  ),
                  DropdownMenuItem(
                    value: 'fan',
                    child: Row(
                      children: [
                        Icon(Icons.toys_rounded, size: 20),
                        SizedBox(width: 8),
                        Text('Fan'),
                      ],
                    ),
                  ),
                ],
                onChanged: (value) {
                  setDialogState(() {
                    selectedType = value!;
                  });
                },
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () async {
                final name = nameController.text.trim();
                if (name.isEmpty) {
                  _showSnackBar('Please enter a device name');
                  return;
                }

                Navigator.pop(context);

                final success = await _apiService.updateDevice(
                  device.uid,
                  name,
                  selectedType,
                );

                if (success) {
                  _showSnackBar('Device updated successfully');
                  _loadDevices();
                } else {
                  _showSnackBar('Failed to update device');
                }
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: AppTheme.primaryIndigo,
              ),
              child: const Text('Save'),
            ),
          ],
        ),
      ),
    );
  }

  void _showAddScheduleDialog() {
    final nameController = TextEditingController();
    TimeOfDay startTime = TimeOfDay.now();
    TimeOfDay endTime = TimeOfDay.now();
    String? selectedDeviceUid;

    if (devices.isEmpty) {
      _showSnackBar('No devices available. Add devices first.');
      return;
    }

    selectedDeviceUid = devices.first.uid;

    showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          backgroundColor: AppTheme.backgroundMedium,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20),
          ),
          title: const Row(
            children: [
              Icon(Icons.schedule_rounded, color: AppTheme.primaryIndigo),
              SizedBox(width: 12),
              Text(
                'Add Schedule',
                style: TextStyle(color: Colors.white),
              ),
            ],
          ),
          content: SingleChildScrollView(
            child: Column(
              mainAxisSize: MainAxisSize.min,
              children: [
                TextField(
                  controller: nameController,
                  style: const TextStyle(color: Colors.white),
                  decoration: InputDecoration(
                    labelText: 'Schedule Name',
                    labelStyle: const TextStyle(color: AppTheme.textSecondary),
                    enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Colors.white.withOpacity(0.3),
                      ),
                      borderRadius: BorderRadius.circular(12),
                    ),
                    focusedBorder: const OutlineInputBorder(
                      borderSide: BorderSide(color: AppTheme.primaryIndigo),
                      borderRadius: BorderRadius.all(Radius.circular(12)),
                    ),
                  ),
                  maxLength: 50,
                ),
                const SizedBox(height: 16),
                DropdownButtonFormField<String>(
                  value: selectedDeviceUid,
                  dropdownColor: AppTheme.backgroundMedium,
                  style: const TextStyle(color: Colors.white),
                  decoration: InputDecoration(
                    labelText: 'Device',
                    labelStyle: const TextStyle(color: AppTheme.textSecondary),
                    enabledBorder: OutlineInputBorder(
                      borderSide: BorderSide(
                        color: Colors.white.withOpacity(0.3),
                      ),
                      borderRadius: BorderRadius.circular(12),
                    ),
                    focusedBorder: const OutlineInputBorder(
                      borderSide: BorderSide(color: AppTheme.primaryIndigo),
                      borderRadius: BorderRadius.all(Radius.circular(12)),
                    ),
                  ),
                  items: devices
                      .map((device) => DropdownMenuItem(
                            value: device.uid,
                            child: Text(device.name),
                          ))
                      .toList(),
                  onChanged: (value) {
                    setDialogState(() {
                      selectedDeviceUid = value;
                    });
                  },
                ),
                const SizedBox(height: 16),
                Row(
                  children: [
                    Expanded(
                      child: OutlinedButton.icon(
                        icon: const Icon(Icons.access_time),
                        label: Text(startTime.format(context)),
                        style: OutlinedButton.styleFrom(
                          foregroundColor: Colors.white,
                          side: BorderSide(
                            color: Colors.white.withOpacity(0.3),
                          ),
                        ),
                        onPressed: () async {
                          final time = await showTimePicker(
                            context: context,
                            initialTime: startTime,
                          );
                          if (time != null) {
                            setDialogState(() {
                              startTime = time;
                            });
                          }
                        },
                      ),
                    ),
                    const SizedBox(width: 8),
                    const Text('-', style: TextStyle(color: Colors.white)),
                    const SizedBox(width: 8),
                    Expanded(
                      child: OutlinedButton.icon(
                        icon: const Icon(Icons.access_time),
                        label: Text(endTime.format(context)),
                        style: OutlinedButton.styleFrom(
                          foregroundColor: Colors.white,
                          side: BorderSide(
                            color: Colors.white.withOpacity(0.3),
                          ),
                        ),
                        onPressed: () async {
                          final time = await showTimePicker(
                            context: context,
                            initialTime: endTime,
                          );
                          if (time != null) {
                            setDialogState(() {
                              endTime = time;
                            });
                          }
                        },
                      ),
                    ),
                  ],
                ),
              ],
            ),
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () async {
                final name = nameController.text.trim();
                if (name.isEmpty) {
                  _showSnackBar('Please enter a schedule name');
                  return;
                }

                Navigator.pop(context);

                final schedule = Schedule(
                  id: '',
                  userId: widget.userId,
                  deviceUid: selectedDeviceUid!,
                  name: name,
                  startTime: '${startTime.hour.toString().padLeft(2, '0')}:${startTime.minute.toString().padLeft(2, '0')}',
                  endTime: '${endTime.hour.toString().padLeft(2, '0')}:${endTime.minute.toString().padLeft(2, '0')}',
                  days: '1111111',
                );

                final success = await _apiService.saveSchedule(schedule);

                if (success) {
                  _showSnackBar('Schedule created successfully');
                  _loadSchedules();
                } else {
                  _showSnackBar('Failed to create schedule');
                }
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: AppTheme.primaryIndigo,
              ),
              child: const Text('Create'),
            ),
          ],
        ),
      ),
    );
  }

  void _showAddSceneDialog() {
    final nameController = TextEditingController();
    final descriptionController = TextEditingController();

    showDialog(
      context: context,
      builder: (context) => AlertDialog(
        backgroundColor: AppTheme.backgroundMedium,
        shape: RoundedRectangleBorder(
          borderRadius: BorderRadius.circular(20),
        ),
        title: const Row(
          children: [
            Icon(Icons.movie_rounded, color: AppTheme.primaryIndigo),
            SizedBox(width: 12),
            Text(
              'Create Scene',
              style: TextStyle(color: Colors.white),
            ),
          ],
        ),
        content: Column(
          mainAxisSize: MainAxisSize.min,
          children: [
            TextField(
              controller: nameController,
              style: const TextStyle(color: Colors.white),
              decoration: InputDecoration(
                labelText: 'Scene Name',
                labelStyle: const TextStyle(color: AppTheme.textSecondary),
                enabledBorder: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Colors.white.withOpacity(0.3),
                  ),
                  borderRadius: BorderRadius.circular(12),
                ),
                focusedBorder: const OutlineInputBorder(
                  borderSide: BorderSide(color: AppTheme.primaryIndigo),
                  borderRadius: BorderRadius.all(Radius.circular(12)),
                ),
              ),
              maxLength: 50,
            ),
            const SizedBox(height: 16),
            TextField(
              controller: descriptionController,
              style: const TextStyle(color: Colors.white),
              decoration: InputDecoration(
                labelText: 'Description (optional)',
                labelStyle: const TextStyle(color: AppTheme.textSecondary),
                enabledBorder: OutlineInputBorder(
                  borderSide: BorderSide(
                    color: Colors.white.withOpacity(0.3),
                  ),
                  borderRadius: BorderRadius.circular(12),
                ),
                focusedBorder: const OutlineInputBorder(
                  borderSide: BorderSide(color: AppTheme.primaryIndigo),
                  borderRadius: BorderRadius.all(Radius.circular(12)),
                ),
              ),
              maxLength: 200,
            ),
          ],
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () async {
              final name = nameController.text.trim();
              if (name.isEmpty) {
                _showSnackBar('Please enter a scene name');
                return;
              }

              Navigator.pop(context);

              final scene = Scene(
                id: '',
                userId: widget.userId,
                name: name,
                devices: [],
              );

              final success = await _apiService.saveScene(scene);

              if (success) {
                _showSnackBar('Scene created successfully');
                _loadScenes();
              } else {
                _showSnackBar('Failed to create scene');
              }
            },
            style: ElevatedButton.styleFrom(
              backgroundColor: AppTheme.primaryIndigo,
            ),
            child: const Text('Create'),
          ),
        ],
      ),
    );
  }

  void _showSettingsDialog() {
    String? selectedDeviceUid;

    if (devices.isEmpty) {
      _showSnackBar('No devices available');
      return;
    }

    selectedDeviceUid = devices.first.uid;

    showDialog(
      context: context,
      builder: (context) => StatefulBuilder(
        builder: (context, setDialogState) => AlertDialog(
          backgroundColor: AppTheme.backgroundMedium,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(20),
          ),
          title: const Row(
            children: [
              Icon(Icons.settings_rounded, color: AppTheme.primaryIndigo),
              SizedBox(width: 12),
              Text(
                'Settings',
                style: TextStyle(color: Colors.white),
              ),
            ],
          ),
          content: Column(
            mainAxisSize: MainAxisSize.min,
            children: [
              DropdownButtonFormField<String>(
                value: selectedDeviceUid,
                dropdownColor: AppTheme.backgroundMedium,
                style: const TextStyle(color: Colors.white),
                decoration: InputDecoration(
                  labelText: 'WiFi Reset Device',
                  labelStyle: const TextStyle(color: AppTheme.textSecondary),
                  enabledBorder: OutlineInputBorder(
                    borderSide: BorderSide(
                      color: Colors.white.withOpacity(0.3),
                    ),
                    borderRadius: BorderRadius.circular(12),
                  ),
                  focusedBorder: const OutlineInputBorder(
                    borderSide: BorderSide(color: AppTheme.primaryIndigo),
                    borderRadius: BorderRadius.all(Radius.circular(12)),
                  ),
                ),
                items: devices
                    .map((device) => DropdownMenuItem(
                          value: device.uid,
                          child: Text(device.name),
                        ))
                    .toList(),
                onChanged: (value) {
                  setDialogState(() {
                    selectedDeviceUid = value;
                  });
                },
              ),
              const SizedBox(height: 16),
              const Text(
                'Resetting WiFi will restart the device in AP mode. You\'ll need to reconfigure the network.',
                style: TextStyle(
                  color: AppTheme.textSecondary,
                  fontSize: 12,
                ),
              ),
            ],
          ),
          actions: [
            TextButton(
              onPressed: () => Navigator.pop(context),
              child: const Text('Cancel'),
            ),
            ElevatedButton(
              onPressed: () async {
                final confirmed = await showDialog<bool>(
                  context: context,
                  builder: (context) => AlertDialog(
                    backgroundColor: AppTheme.backgroundMedium,
                    title: const Text(
                      'Confirm WiFi Reset',
                      style: TextStyle(color: Colors.white),
                    ),
                    content: const Text(
                      'Are you sure you want to reset WiFi for this device?',
                      style: TextStyle(color: AppTheme.textSecondary),
                    ),
                    actions: [
                      TextButton(
                        onPressed: () => Navigator.pop(context, false),
                        child: const Text('Cancel'),
                      ),
                      ElevatedButton(
                        onPressed: () => Navigator.pop(context, true),
                        style: ElevatedButton.styleFrom(
                          backgroundColor: AppTheme.errorRed,
                        ),
                        child: const Text('Reset'),
                      ),
                    ],
                  ),
                );

                if (confirmed == true && selectedDeviceUid != null) {
                  Navigator.pop(context);

                  final success =
                      await _apiService.resetWifi(selectedDeviceUid!);

                  if (success) {
                    _showSnackBar('WiFi reset command sent');
                  } else {
                    _showSnackBar('Failed to reset WiFi');
                  }
                }
              },
              style: ElevatedButton.styleFrom(
                backgroundColor: AppTheme.errorRed,
              ),
              child: const Text('Reset WiFi'),
            ),
          ],
        ),
      ),
    );
  }

  Future<void> _deleteSchedule(String scheduleId) async {
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        backgroundColor: AppTheme.backgroundMedium,
        title: const Text(
          'Delete Schedule',
          style: TextStyle(color: Colors.white),
        ),
        content: const Text(
          'Are you sure you want to delete this schedule?',
          style: TextStyle(color: AppTheme.textSecondary),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () => Navigator.pop(context, true),
            style: ElevatedButton.styleFrom(
              backgroundColor: AppTheme.errorRed,
            ),
            child: const Text('Delete'),
          ),
        ],
      ),
    );

    if (confirmed == true) {
      final success = await _apiService.deleteSchedule(scheduleId);

      if (success) {
        _showSnackBar('Schedule deleted');
        _loadSchedules();
      } else {
        _showSnackBar('Failed to delete schedule');
      }
    }
  }

  Future<void> _activateScene(String sceneId) async {
    final success = await _apiService.activateScene(sceneId);

    if (success) {
      _showSnackBar('Scene activated successfully');
      _loadDevices(); // Refresh devices to show updated states
    } else {
      _showSnackBar('Failed to activate scene');
    }
  }

  Future<void> _deleteScene(String sceneId) async {
    final confirmed = await showDialog<bool>(
      context: context,
      builder: (context) => AlertDialog(
        backgroundColor: AppTheme.backgroundMedium,
        title: const Text(
          'Delete Scene',
          style: TextStyle(color: Colors.white),
        ),
        content: const Text(
          'Are you sure you want to delete this scene?',
          style: TextStyle(color: AppTheme.textSecondary),
        ),
        actions: [
          TextButton(
            onPressed: () => Navigator.pop(context, false),
            child: const Text('Cancel'),
          ),
          ElevatedButton(
            onPressed: () => Navigator.pop(context, true),
            style: ElevatedButton.styleFrom(
              backgroundColor: AppTheme.errorRed,
            ),
            child: const Text('Delete'),
          ),
        ],
      ),
    );

    if (confirmed == true) {
      final success = await _apiService.deleteScene(sceneId);

      if (success) {
        _showSnackBar('Scene deleted');
        _loadScenes();
      } else {
        _showSnackBar('Failed to delete scene');
      }
    }
  }

  void _showSnackBar(String message) {
    if (mounted) {
      ScaffoldMessenger.of(context).showSnackBar(
        SnackBar(
          content: Text(message),
          backgroundColor: AppTheme.backgroundMedium,
          behavior: SnackBarBehavior.floating,
          shape: RoundedRectangleBorder(
            borderRadius: BorderRadius.circular(10),
          ),
        ),
      );
    }
  }
}
