## speed control menu
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class SpeedScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        layout.add_widget(Label(text="Set Print Speed (mm/min)"))

        speeds = [str(v) for v in [0.25, 0.5, 0.75, 1, 1.25, 1.5, 1.75, 2.0]]

        for s in speeds:
            layout.add_widget(Button(text=f"{s} mm/min"))

        def go_home(instance):
            self.manager.current = 'home'

        layout.add_widget(Button(text="Back", on_press=go_home))
        self.add_widget(layout)
