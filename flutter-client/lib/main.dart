import 'package:flutter/material.dart';

import 'package:beamer/beamer.dart';

import 'package:vircon/common/routes.dart';
import 'package:vircon/features/connect/connect_page.dart';
import 'package:vircon/features/connect/home_page.dart';
import 'package:vircon/features/controller/controller_page.dart';

void main() => runApp(MainApp());

class MainApp extends StatelessWidget {
  final _routerDelegate = BeamerDelegate(
    locationBuilder: RoutesLocationBuilder(
      routes: {
        HomeRoute.route: (context, state, data) => HomePage(),
        ConnectRoute.route: (context, state, data) {
          return ConnectPage(
            host: state.pathParameters[ConnectRoute.host],
            port: state.pathParameters[ConnectRoute.port],
          );
        },
        ControllerRoute.route: (context, state, data) => ControllerPage()
      },
    ).call,
  );

  MainApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp.router(
      routeInformationParser: BeamerParser(),
      routerDelegate: _routerDelegate,
      backButtonDispatcher:
          BeamerBackButtonDispatcher(delegate: _routerDelegate),
    );
  }
}
