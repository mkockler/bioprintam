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
        layout = BoxLayout(orientation='vertical', padding=10, spacing=10)

        def nav_button(text, target_screen):
            return Button(
                text=text,
                on_press=lambda x: setattr(self.manager, 'current', target_screen)
            )

        # clickable buttons
        layout.add_widget(nav_button("Set Temperature: ", "temperature"))
        layout.add_widget(nav_button("Set Speed", "speed"))
        layout.add_widget(nav_button("Set Pressure", "pressure"))
        layout.add_widget(nav_button("Set Start Conc", "start_conc"))
        layout.add_widget(nav_button("Set End Conc", "end_conc"))
        layout.add_widget(nav_button("Set Volume S1", "volume_s1"))
        layout.add_widget(nav_button("Set Volume S2", "volume_s2"))
        layout.add_widget(nav_button("Set Print Time", "print_time"))

        layout.add_widget(Button(text="Locked", background_color=(0.5, 0.5, 1, 1)))

        self.add_widget(layout)



class MainApp(App):
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
