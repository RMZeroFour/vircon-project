class HomeRoute {
  static const String route = '/';
  static String uri() => route;
}

class ConnectRoute {
  static const String host = 'host';
  static const String port = 'port';
  static const String route = '/connect/:$host/:$port';
  static String uri(String host, String port) => '/connect/$host/:$port';
}

class ControllerRoute {
  static const String route = '/controller';
  static String uri() => route;
}
