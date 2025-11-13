<template>
	<button class="category-button">
		<slot></slot>
	</button>
</template>

<script>
export default {
	name: 'category-button',
	props: {
		category: {
			type: Object,
			required: true
		}
	}
};
</script>

<style scoped>

</style>