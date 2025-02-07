import 'package:flutter/material.dart';

import 'package:beamer/beamer.dart';

import 'package:vircon/common/routes.dart';

class ConnectPage extends StatelessWidget {
  final String? host;
  final String? port;

  const ConnectPage({super.key, this.host, this.port});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: Text('Connect Page $host:$port'),
      ),
      body: ElevatedButton(
        onPressed: () => context.beamToNamed(ControllerRoute.uri()),
        child: const Text('Open'),
      ),
    );
  }
}
