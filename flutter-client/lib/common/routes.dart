class HomeRoute {
  static const String route = '/';
  static const String host = 'host';
  static const String port = 'port';
  static String uri(String? host, String? port) {
    return Uri(path: route, queryParameters: {
      HomeRoute.host: host,
      HomeRoute.port: port,
    }).toString();
  }
}

class CouldNotConnectRoute {
  static const String route = '/failed';
  static String uri() => route;
}

class ControllerRoute {
  static const String route = '/controller';
  static String uri() => route;
}
