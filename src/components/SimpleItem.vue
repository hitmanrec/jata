<!-- simplified item representation for item list -->
<template>
	<div class="item">
		<div class="item-content">
			<div class="item-header">
				<div class="title">
					{{ item.title }}
				</div>
				<div class="status">
					{{ item.status }}
				</div>
			</div>
			<div v-if="item.tags.length > 0" class="tags">
				Tags: {{ item.tags.join(', ') }}
			</div>
			<div class="description">
				{{ item.description.substring(0, 100) }}{{ item.description.length > 100 ? '...' : '' }}
			</div>
			<div v-if="item.status === 'planned' || item.status === 'started'" class="expectation">
				Expectation: {{ item.expectation }}
			</div>
			<div v-if="item.status === 'complete'|| item.status === 'dropped'" class="rating">
				Rating: {{ item.rating }}
			</div>
		</div>
		<div class="item-btns">
			<small-button class="delete-button"
				@click="$emit('delete-item', item)"
			>
				Delete
			</small-button>
		</div>
	</div>
</template>

<script>
export default {
	props: {
		item: {
			type: Object,
			required: true
		}
	}
}
</script>

<style scoped>
.item{
	padding: 10px;
	border: 2px solid var(--accent-col);
	border-radius: var(--big-border-rad);
	display: flex;
	flex-direction: column;
}

.item-content{
	display: flex;
	flex-direction: column;
	justify-content: space-between;
}

.title{
	font-family: var(--strong-font);
	font-weight: var(--main-font-weight);
	font-size: 1.2em;
}

.status{
	align-self: flex-end;
}

.rating{
	align-self: flex-end;
}

.expectation{
	align-self: flex-end;
}

.item-header{
	display: flex;
	justify-content: space-between;
	align-items: center;
	margin-bottom: 5px;
}

.item-btns{
	margin-top: 10px;
	display: flex;
	flex-direction: column;
	align-items: flex-end;
}
</style>