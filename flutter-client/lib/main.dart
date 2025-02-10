import 'package:flutter/material.dart';

import 'package:beamer/beamer.dart';

import 'package:vircon/common/routes.dart';
import 'package:vircon/features/connect/could_not_connect_page.dart';
import 'package:vircon/features/connect/home_page.dart';
import 'package:vircon/features/controller/controller_page.dart';

void main() => runApp(MainApp());

class MainApp extends StatelessWidget {
  final _routerDelegate = BeamerDelegate(
    locationBuilder: RoutesLocationBuilder(
      routes: {
        HomeRoute.route: (context, state, data) {
          return HomePage(
            host: state.queryParameters[HomeRoute.host],
            port: state.queryParameters[HomeRoute.port],
          );
        },
        CouldNotConnectRoute.route: (context, state, data) =>
            CouldNotConnectPage(),
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
