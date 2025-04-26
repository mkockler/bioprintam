# main.py
from kivy.app import App
from kivy.uix.screenmanager import ScreenManager, Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.label import Label
from kivy.uix.button import Button
from kivy.uix.gridlayout import GridLayout
from kivy.uix.textinput import TextInput

# Import screens from other files
from temp import TemperatureScreen
from speed import SpeedScreen
from pressure import PressureScreen
from start_conc import StartConcScreen
from end_conc import EndConcScreen
from volume_s1 import VolumeS1Screen
from volume_s2 import VolumeS2Screen
from print_time import PrintTimeScreen

class HomeScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        self.layout = BoxLayout(orientation='vertical', padding=10, spacing=10)

        self.nav_buttons = []
        self.value_labels = {}  # Dictionary to store value labels for updating later

        def nav_row(text, target_screen, value_key):
            row = BoxLayout(orientation='horizontal', size_hint_y=None, height=50)

            btn = Button(
                text=text,
                size_hint_x=0.6,
                on_press=lambda x: self.go_to_screen(target_screen)
            )
            self.nav_buttons.append(btn)

            value_label = Label(text="Not set", size_hint_x=0.4)
            self.value_labels[value_key] = value_label

            row.add_widget(btn)
            row.add_widget(value_label)

            return row

        # Add navigation rows
        self.layout.add_widget(nav_row("Set Temperature", "temperature", "temperature"))
        self.layout.add_widget(nav_row("Set Speed", "speed", "speed"))
        self.layout.add_widget(nav_row("Set Pressure", "pressure", "pressure"))
        self.layout.add_widget(nav_row("Set Start Conc", "start_conc", "start_conc"))
        self.layout.add_widget(nav_row("Set End Conc", "end_conc", "end_conc"))
        self.layout.add_widget(nav_row("Set Volume S1", "volume_s1", "volume_s1"))
        self.layout.add_widget(nav_row("Set Volume S2", "volume_s2", "volume_s2"))
        self.layout.add_widget(nav_row("Set Print Time", "print_time", "print_time"))

        # Lock toggle button
        self.locked = True
        self.lock_button = Button(
            text="Locked",
            background_color=(0.5, 0.5, 1, 1)  # Light blue
        )
        self.lock_button.bind(on_press=self.toggle_lock)
        self.layout.add_widget(self.lock_button)

        self.add_widget(self.layout)
        self.update_nav_buttons()

    def toggle_lock(self, instance):
        self.locked = not self.locked

        if self.locked:
            self.lock_button.text = "Locked"
            self.lock_button.background_color = (0.5, 0.5, 1, 1)  # Light blue
        else:
            self.lock_button.text = "Unlocked"
            self.lock_button.background_color = (0.5, 1, 0.5, 1)  # Light green

        self.update_nav_buttons()

    def update_nav_buttons(self):
        for btn in self.nav_buttons:
            btn.disabled = self.locked

    def go_to_screen(self, screen_name):
        if not self.locked:
            self.manager.current = screen_name

    def on_pre_enter(self, *args):
        # Update value labels every time you come back to the home screen
        app = App.get_running_app()

        for key, label in self.value_labels.items():
            value = getattr(app, key, None)
            if value:
                label.text = str(value)
            else:
                label.text = "Not set"

class MainApp(App):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)
        # Initialize global variables to hold selected values
        self.temperature = None
        self.speed = None
        self.pressure = None
        self.start_conc = None
        self.end_conc = None
        self.volume_s1 = None
        self.volume_s2 = None
        self.print_time = None

    def build(self):
        sm = ScreenManager()
        sm.add_widget(HomeScreen(name='home'))
        sm.add_widget(TemperatureScreen(name='temperature'))
        sm.add_widget(SpeedScreen(name='speed'))
        sm.add_widget(PressureScreen(name='pressure'))
        sm.add_widget(StartConcScreen(name='start_conc'))
        sm.add_widget(EndConcScreen(name='end_conc'))
        sm.add_widget(VolumeS1Screen(name='volume_s1'))
        sm.add_widget(VolumeS2Screen(name='volume_s2'))
        sm.add_widget(PrintTimeScreen(name='print_time'))
        return sm

if __name__ == '__main__':
    MainApp().run()
