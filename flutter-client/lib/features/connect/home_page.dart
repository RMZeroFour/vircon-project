import 'package:flutter/material.dart';

import 'package:beamer/beamer.dart';

import 'package:vircon/common/routes.dart';
import 'package:vircon/features/connect/connection_details_form.dart';

class HomePage extends StatelessWidget {
  final String? host;
  final String? port;

  const HomePage({super.key, this.host, this.port});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home Page'),
      ),
      body: ConnectionDetailsForm(
        initialHost: host,
        initialPort: port,
        onConnect: (String host, int port) async {
          final bool connected = await _tryConnect();
          if (context.mounted) {
            if (connected) {
              context.beamToNamed(ControllerRoute.uri());
            } else {
              context.beamToNamed(CouldNotConnectRoute.uri());
            }
          }
        },
      ),
    );
  }

  Future<bool> _tryConnect() async {
    return true;
  }
}
