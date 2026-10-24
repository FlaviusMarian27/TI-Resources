import { Button, Pressable, ScrollView, StyleSheet } from 'react-native';
import { useRouter } from 'expo-router';
import ActivityCard from '../../components/ActivityCard';
import { activitati } from '../../data/activitati';

export default function Activitati() {
  const router = useRouter();

  return (
    <ScrollView contentContainerStyle={styles.container}>
      <Button title="Adaugă activitate" onPress={() => router.push('/adauga')} />

      {activitati.map((a) => (
        <Pressable
          key={a.id}
          onPress={() => router.push({ pathname: '/[id]', params: { id: a.id } })}
        >
          <ActivityCard
            title={a.title}
            description={a.description}
            category={a.category}
          />
        </Pressable>
      ))}
    </ScrollView>
  );
}

const styles = StyleSheet.create({
  container: {
    padding: 16,
    gap: 12,
  },
});