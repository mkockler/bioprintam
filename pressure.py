## pressure menu
from kivy.uix.screenmanager import Screen
from kivy.uix.boxlayout import BoxLayout
from kivy.uix.button import Button
from kivy.uix.label import Label

class PressureScreen(Screen):
    def __init__(self, **kwargs):
        super().__init__(**kwargs)

        layout = BoxLayout(orientation='vertical', spacing=10, padding=10)
        layout.add_widget(Label(text="Set Pressure (psi)"))

        pressures = [str(v) for v in range(5, 55, 5)]  # 5, 10, ..., 50

        for p in pressures:
            layout.add_widget(Button(text=f"{p} psi"))

        def go_home(instance):
            self.manager.current = 'home'

        layout.add_widget(Button(text="Back", on_press=go_home))
        self.add_widget(layout)
