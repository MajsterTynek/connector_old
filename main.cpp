int main(int argc, char* argv[])
{
  try
  {
    if (argc != 3)
    {
      std::cout << "Usage: connector <host> <port>\n";
      std::cout << "Example:\n";
      std::cout << "  connector 2b2t.org 25565\n";
      return 1;
    }

    asio::io_service io_service;
    client c(io_service, argv[1], argv[2]);
    io_service.run();
  }
  catch (std::exception& e)
  {
    std::cout << "Exception: " << e.what() << "\n";
  }

  return 0;
}