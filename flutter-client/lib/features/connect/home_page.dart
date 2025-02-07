import 'package:flutter/material.dart';

import 'package:beamer/beamer.dart';

import 'package:vircon/common/routes.dart';

class HomePage extends StatelessWidget {
  const HomePage({super.key});

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        title: const Text('Home Page'),
      ),
      body: ElevatedButton(
        onPressed: () =>
            context.beamToNamed(ConnectRoute.uri('192.168.0.1', '12345')),
        child: const Text('Connect'),
      ),
    );
  }
}
