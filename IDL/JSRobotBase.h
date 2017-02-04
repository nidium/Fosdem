class JSRobotBase
{
  public:
    JSRobotBase(char *name) : m_Name(strdup(name)) { }

    ~JSRobotBase() {
        free(m_Name);
    }

  protected:
    void echo(char *str) {
        printf("%s : says \"%s\"\n", m_Name, str);
    }

  private:
    char *m_Name = nullptr;
};
