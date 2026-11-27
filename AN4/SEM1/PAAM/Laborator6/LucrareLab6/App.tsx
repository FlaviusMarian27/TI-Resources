import DateTimePicker, {
  DateTimePickerAndroid,
  DateTimePickerEvent,
} from '@react-native-community/datetimepicker';
import * as Notifications from 'expo-notifications';
import { useEffect, useState } from 'react';
import { Alert, Button, Platform, ScrollView, StyleSheet, Text, TextInput, View } from 'react-native';

// Cum se comportă notificarea când aplicația e deschisă (foreground)
Notifications.setNotificationHandler({
  handleNotification: async () => ({
    shouldShowBanner: true,
    shouldShowList: true,
    shouldPlaySound: true,
    shouldSetBadge: false,
  }),
});

const creeazaCanal = async () => {
  try {
    await Notifications.setNotificationChannelAsync('default', {
      name: 'Remindere',
      importance: Notifications.AndroidImportance.MAX,
    });
  } catch (e) {
    console.log('Canalul nu a putut fi creat în Expo Go:', e);
  }
};

const doua = (n: number) => n.toString().padStart(2, '0');
const formateazaData = (d: Date) =>
  `${doua(d.getDate())}.${doua(d.getMonth() + 1)}.${d.getFullYear()}`;
const formateazaOra = (d: Date) => `${doua(d.getHours())}:${doua(d.getMinutes())}`;

export default function App() {
  const [titlu, setTitlu] = useState('');
  const [data, setData] = useState(() => {
    const d = new Date(Date.now() + 2 * 60 * 1000); 
    d.setSeconds(0, 0);
    return d;
  });
  const [permisiune, setPermisiune] = useState<string>('undetermined');

  useEffect(() => {
    const init = async () => {
      if (Platform.OS === 'android') {
        await creeazaCanal();
      }
      const { status } = await Notifications.getPermissionsAsync();
      setPermisiune(status);
    };
    init();
  }, []);

  const verificaPermisiunea = async () => {
    const { status } = await Notifications.getPermissionsAsync();
    setPermisiune(status);
  };

  const cerePermisiunea = async () => {
    if (Platform.OS === 'android') {
      await creeazaCanal();
    }
    const { status } = await Notifications.requestPermissionsAsync();
    setPermisiune(status);
    return status === 'granted';
  };

  
  const alegeData = () => {
    DateTimePickerAndroid.open({
      value: data,
      mode: 'date',
      onChange: (event: DateTimePickerEvent, ales?: Date) => {
        if (event.type === 'set' && ales) {
          const nou = new Date(data);
          nou.setFullYear(ales.getFullYear(), ales.getMonth(), ales.getDate());
          setData(nou);
        }
      },
    });
  };

  const alegeOra = () => {
    DateTimePickerAndroid.open({
      value: data,
      mode: 'time',
      is24Hour: true,
      onChange: (event: DateTimePickerEvent, ales?: Date) => {
        if (event.type === 'set' && ales) {
          const nou = new Date(data);
          nou.setHours(ales.getHours(), ales.getMinutes(), 0, 0);
          setData(nou);
        }
      },
    });
  };

  const programeaza = async () => {
    if (titlu.trim() === '') {
      Alert.alert('Eroare', 'Introdu titlul activității.');
      return;
    }
    if (data.getTime() <= Date.now()) {
      Alert.alert('Eroare', 'Alege un moment din viitor.');
      return;
    }

    const acordata = permisiune === 'granted' || (await cerePermisiunea());
    if (!acordata) {
      Alert.alert('Permisiune refuzată', 'Fără permisiune nu pot programa notificarea.');
      return;
    }

    await Notifications.scheduleNotificationAsync({
      content: {
        title: 'Reminder',
        body: titlu.trim(),
      },
      trigger: {
        type: Notifications.SchedulableTriggerInputTypes.DATE,
        date: data,
        channelId: 'default',
      },
    });

    Alert.alert(
      'Notificare programată',
      `„${titlu.trim()}” – ${formateazaData(data)}, ora ${formateazaOra(data)}.`
    );
  };

  const textPermisiune =
    permisiune === 'granted'
      ? 'acordată'
      : permisiune === 'denied'
      ? 'refuzată'
      : 'nesolicitată încă';

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Text style={styles.titlu}>Reminder</Text>

      <Text style={styles.sectiune}>1. Permisiune</Text>
      <Text style={styles.text}>Stare: {textPermisiune}</Text>
      <Button title="Cere permisiunea" onPress={cerePermisiunea} />
      <Button title="Verifică permisiunea" onPress={verificaPermisiunea} />

      <Text style={styles.sectiune}>2. Activitate</Text>
      <TextInput
        style={styles.input}
        value={titlu}
        onChangeText={setTitlu}
        placeholder="Titlul activității (ex: Ședință proiect)"
      />

      <Text style={styles.sectiune}>3. Data și ora</Text>
      <Text style={styles.text}>
        {formateazaData(data)}, ora {formateazaOra(data)}
      </Text>

      {Platform.OS === 'android' ? (
        <View style={styles.rand}>
          <Button title="Alege data" onPress={alegeData} />
          <Button title="Alege ora" onPress={alegeOra} />
        </View>
      ) : (
        <DateTimePicker
          value={data}
          mode="datetime"
          onChange={(event: DateTimePickerEvent, ales?: Date) => {
            if (ales) setData(ales);
          }}
        />
      )}

      <Button title="Programează notificarea" onPress={programeaza} />
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 16,
    paddingTop: 56,
    gap: 12,
  },
  titlu: {
    fontSize: 28,
    fontWeight: 'bold',
  },
  sectiune: {
    fontSize: 18,
    fontWeight: '600',
    marginTop: 8,
  },
  text: {
    fontSize: 16,
    color: '#555',
  },
  input: {
    borderWidth: 1,
    borderColor: '#ccc',
    borderRadius: 8,
    padding: 10,
    fontSize: 16,
  },
  rand: {
    flexDirection: 'row',
    gap: 12,
  },
});
